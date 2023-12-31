import { output, input } from "./index.js";

const { d13, a27 } = output("192.168.1.138", "d13", "a27");

d13.set("high");
a27.set(2);

async function readDigitalInput() {
  const { d3 } = input("192.168.1.138", "d3");
  try {
    const response = await d3.get();
    console.log("Digital Input Response:", response);
  } catch (err) {
    console.error("Error reading digital input:", err);
  }
}

readDigitalInput();

async function readAnalogInput() {
  const { a34 } = input("192.168.1.138", "a34");

  try {
    const response = await a34.get("voltage", 12);
    console.log("Analog Input Response:", response);
  } catch (err) {
    console.error("Error reading analog input:", err);
  }
}

readAnalogInput();
