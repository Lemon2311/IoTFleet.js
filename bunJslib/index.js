class IO {
    constructor(pin, mode) {
        this.pin = pin.slice(1);

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
        const initializeDigitalPinUrl = "http://192.168.1.138/initializeDigitalPin";
        const data = { pin: pin, mode: mode };
        const fullUrl = `${initializeDigitalPinUrl}?${new URLSearchParams(data).toString()}`;
        await this.#postApiData(fullUrl).then((data) => console.log(data));
    }

    // Protected method for subclasses
    async _post(url) {
        return await this.#postApiData(url);
    }
}

class Output extends IO {
    constructor(pin) {
        super(pin, "output");

        this.set = async (state) => {
            await this.#digitalPinOutput(this.pin, state);
        };

    }

    async #digitalPinOutput(pin, state) {
        const digitalOutputUrl = "http://192.168.1.138/digitalOutput";
        const data = { pin: pin, state: state };
        const fullUrl = `${digitalOutputUrl}?${new URLSearchParams(data).toString()}`;
        await this._post(fullUrl).then((data) => console.log(data));
    }
}

class OutputFactory {
    constructor(...pins) {
        this.outputs = {};

        pins.forEach(pin => {
            this.outputs[pin] = new Output(pin);
        });
    }

    getOutput(pin)//should be done async
    {
        return this.outputs[pin];
    }
}

const output = (...pins) => {
    const factory = new OutputFactory(...pins);
    const outputs = {};

    pins.forEach(pin => {
        outputs[pin] = factory.getOutput(pin);
    });

    return outputs;
};

export default output;

