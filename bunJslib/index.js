class IO {
    constructor(pin, mode, ip) {
        this.pin = pin.slice(1);
        this.ip = ip;

        (async () => {
            if (pin.charAt(0) === "d") {
                await this.#initializeDigitalPin(this.pin, mode);
            }
        })();
    }

    async #postApiData(url) {
        try {
            const response = await fetch(url, {
                method: "POST",
                headers: {
                    "Content-Type": "text/plain",
                },
            });

            if (!response.ok) {
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
            return await response.text();
        } catch (error) {
            console.error("Error posting data:", error);
        }
    }

    async #initializeDigitalPin(pin, mode) {
        const initializeDigitalPinUrl = `http://${this.ip}/initializeDigitalPin`;
        const data = { pin: pin, mode: mode };
        const fullUrl = `${initializeDigitalPinUrl}?${new URLSearchParams(data).toString()}`;
        await this.#postApiData(fullUrl).then((data) => console.log(data));
    }

    async _post(url) {
        return await this.#postApiData(url);
    }
}

class Output extends IO {
    constructor(pin, ip) {
        super(pin, "output", ip);
    }

    set = async (state) => {
        await this.#digitalPinOutput(this.pin, state);
    }

    async #digitalPinOutput(pin, state) {
        const digitalOutputUrl = `http://${this.ip}/digitalOutput`;
        const data = { pin: pin, state: state };
        const fullUrl = `${digitalOutputUrl}?${new URLSearchParams(data).toString()}`;
        await this._post(fullUrl).then((data) => console.log(data));
    }
}

class OutputFactory {
    constructor(ip, ...pins) {
        this.outputs = {};
        this.ip = ip;

        pins.forEach(pin => {
            this.outputs[pin] = new Output(pin, this.ip);
        });
    }

    getOutput(pin) {
        return this.outputs[pin];
    }
}

const output = (ip, ...pins) => {
    const factory = new OutputFactory(ip, ...pins);
    const outputs = {};

    pins.forEach(pin => {
        outputs[pin] = factory.getOutput(pin);
    });

    return outputs;
};

export default output;
