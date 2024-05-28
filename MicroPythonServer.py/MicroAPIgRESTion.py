import uasyncio as asyncio
import network
from machine import Pin
from WIFI_CREDENTIALS import SSID, PASS

# Connect to WiFi
def connect_wifi(ssid, password):
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    if not wlan.isconnected():
        print('Connecting to WiFi...')
        wlan.connect(ssid, password)
        while not wlan.isconnected():
            pass
    print('Connected to WiFi:', wlan.ifconfig())

# Dictionary to store URL handlers
url_handlers = {}

def parse_query_params(query_string):
    query_params = {}
    for param in query_string.split('&'):
        if '=' in param:
            key, value = param.split('=', 1)
            query_params[key] = value
    return query_params

def route(url, method='GET', *query_params):
    def decorator(handler):
        async def wrapper(request_url, reader, writer):
            # Parse query parameters
            query_params_dict = parse_query_params(request_url.split('?', 1)[-1])

            # Check if the required query parameters are present
            if all(param in query_params_dict for param in query_params):
                # Call the handler function with query parameters
                response = await handler(**query_params_dict)
            else:
                # Send a 400 response if required query parameters are missing
                response = 'HTTP/1.0 400 Bad Request\r\nContent-Type: text/html\r\nAccess-Control-Allow-Origin: *\r\n\r\nMissing query parameters\r\n'
                await writer.awrite(response.encode())
                await writer.aclose()
                return

            # Write the response
            response = f'HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nAccess-Control-Allow-Origin: *\r\n\r\n{response}\r\n'
            await writer.awrite(response.encode())

            # Close the connection
            await writer.aclose()

        # Register the wrapper function instead of the handler function
        url_handlers[(url, method, query_params)] = wrapper
        return handler
    return decorator

def GET(url, *query_params):
    return route(url, 'GET', *query_params)

def POST(url, *query_params):
    return route(url, 'POST', *query_params)

def PUT(url, *query_params):
    return route(url, 'PUT', *query_params)

def DELETE(url, *query_params):
    return route(url, 'DELETE', *query_params)

def PATCH(url, *query_params):
    return route(url, 'PATCH', *query_params)

# Dispatch incoming HTTP requests to the appropriate handler
async def dispatch_request(reader, writer):
    print('Received request from:', writer.get_extra_info('peername'))

    # Read the first line of the request
    line = await reader.readline()
    line = line.decode().strip()
    method, url, version = line.split()

    # Read the entire request
    while True:
        line = await reader.readline()
        line = line.decode().strip()
        if line == '':
            break
        print('Received header:', line)

    # Extract the path (without query string) from the URL
    path = url.split('?', 1)[0]

    # Parse query parameters
    query_params = parse_query_params(url.split('?', 1)[-1])

    # Call the appropriate handler function
    for key in url_handlers:
        if key[0] == path and key[1] == method and all(param in query_params for param in key[2]):
            await url_handlers[key](url, reader, writer)
            return

    # Send a 404 response if no handler is found
    response = b'HTTP/1.0 404 Not Found\r\n\r\n'
    await writer.awrite(response)
    await writer.aclose()

# Start the async REST API server
async def main():
    # Connect to WiFi
    connect_wifi(SSID, PASS)
    
    # Start server
    server = await asyncio.start_server(dispatch_request, '0.0.0.0', 80)

    print('Server listening on IP: 0.0.0.0')
    print('Server listening on port: 80')

    async with server:
        await server.wait_closed()