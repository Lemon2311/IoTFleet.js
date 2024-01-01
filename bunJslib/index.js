class IO {
  constructor(pin, mode, ip) {
    this.type = pin.charAt(0);
    this.pin = pin.slice(1);
    this.ip = ip;
    this.initialized = this.initializePin(mode);
  }

  async initializePin(mode) {
    if (this.type === "d") {
      this.#initializeDigitalPin(this.pin, mode);
    }
  }

  async #ApiData(url, type) {
    try {
      const response = await fetch(url, {
        method: type,
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
    const fullUrl = `${initializeDigitalPinUrl}?${new URLSearchParams(
      data
    ).toString()}`;
    await this.#ApiData(fullUrl, "POST").then((data) => console.log(data));
  }

  async _post(url) {
    return await this.#ApiData(url, "POST");
  }

  async _get(url) {
    return await this.#ApiData(url, "GET");
  }
}

class Output extends IO {
  constructor(pin, ip) {
    super(pin, "output", ip);
  }

  set = async (state) => {
    await this.initialized; // Wait for initialization to complete
    if (this.type === "d") {
      await this.#digitalPinOutput(this.pin, state);
    } else if (this.type === "a") {
      await this.#analogPinOutput(this.pin, "voltage", state);
    }
  };

  async #digitalPinOutput(pin, state) {
    const digitalOutputUrl = `http://${this.ip}/digitalOutput`;
    const data = { pin: pin, state: state };
    const fullUrl = `${digitalOutputUrl}?${new URLSearchParams(
      data
    ).toString()}`;
    await this._post(fullUrl).then((data) => console.log(data));
  }

  #analogPinOutput = async (pin, type, value) => {
    const analogOutputUrl = `http://${this.ip}/analogOutput`;
    const data = { pin: pin, type: type, value: value };
    const fullUrl = `${analogOutputUrl}?${new URLSearchParams(
      data
    ).toString()}`;
    await this._post(fullUrl).then((data) => console.log(data));
  };
}

class Input extends IO {
  constructor(pin, ip) {
    super(pin, "input", ip);
  }

  get = async (type = "voltage", precision = 12) => {
    await this.initialized; // Wait for initialization to complete
    try {
      let response;
      if (this.type === "d") {
        response = await this.#digitalPinInput(this.pin);
      } else if (this.type === "a") {
        response = await this.#analogPinInput(this.pin, type, precision);
      }
      console.log(response);
    } catch (error) {
      console.error("Error:", error);
    }
  };

  async #digitalPinInput(pin) {
    const digitalInputUrl = `http://${this.ip}/digitalInput`;
    const data = { pin: pin };
    const fullUrl = `${digitalInputUrl}?${new URLSearchParams(
      data
    ).toString()}`;
    return await this._get(fullUrl);
  }

  #analogPinInput = async (pin, type, precision) => {
    const analogInputUrl = `http://${this.ip}/analogInput`;
    const data = { pin: pin, type: type, precision: precision };
    const fullUrl = `${analogInputUrl}?${new URLSearchParams(data).toString()}`;
    return await this._get(fullUrl);
  };
}

class OutputFactory {
  constructor(ip, ...pins) {
    this.outputs = {};
    this.ip = ip;

    pins.forEach((pin) => {
      this.outputs[pin] = new Output(pin, this.ip);
    });
  }

  getOutput(pin) {
    return this.outputs[pin];
  }
}

class InputFactory {
  constructor(ip, ...pins) {
    this.inputs = {};
    this.ip = ip;

    pins.forEach((pin) => {
      this.inputs[pin] = new Input(pin, this.ip);
    });
  }

  getInput(pin) {
    return this.inputs[pin];
  }
}

const output = (ip, ...pins) => {
  const factory = new OutputFactory(ip, ...pins);
  const outputs = {};

  pins.forEach((pin) => {
    outputs[pin] = factory.getOutput(pin);
  });

  return outputs;
};

const input = (ip, ...pins) => {
  const factory = new InputFactory(ip, ...pins);
  const inputs = {};

  pins.forEach((pin) => {
    inputs[pin] = factory.getInput(pin);
  });

  return inputs;
};

export { output, input };
