import { output } from "../lib.js/script.js";

// Get the add-item button element
const addItemButton = document.getElementById("add");

// Get the list element
const itemList = document.getElementById("item-list");

// Get the input element
const inputField = document.getElementById("new-item");

// Create an array to store the digital outputs
let outputs = [];

let names = [];

// Function to handle the add-item button click event
function handleAddItemClick() {
  // Get the value of the input field
  const newItemText = inputField.value;

  // Get the index of the first '/' character
  const slashIndex = newItemText.indexOf("/");

  // Get the text before the '/'
  const beginningText = newItemText.substring(0, slashIndex);

  // Clear the input field
  inputField.value = "";

  // Create a new button element
  const newItem = document.createElement("button");

  let name = newItemText.substring(
    slashIndex + 1,
    newItemText.length
  )

  // Set the text of the button
  newItem.textContent = name;

  newItem.id = outputs.length;

  // Add the button to the list
  itemList.appendChild(newItem);

  const out = output("192.168.1.138", beginningText)[beginningText];

  // Add the output to the outputs array
  outputs.push(out);

  names.push(name);

  localStorage.setItem("outputs", JSON.stringify(outputs));

  localStorage.setItem("names", JSON.stringify(names));

  // Add a click event listener to the new button, to toggle the output
  let clickCount = 0;
  newItem.addEventListener("click", function () {
    clickCount++;
    if (clickCount % 2 === 1) {
      outputs[newItem.id].set("high");
    } else {
      outputs[newItem.id].set("low");
    }
  });
}

// Load the outputs from local storage when the page loads
window.onload = function () {
  const savedOutputs = JSON.parse(localStorage.getItem("outputs"));
  const savedNames = JSON.parse(localStorage.getItem("names"));
  console.log(savedOutputs);
  console.log('dasda');
  console.log(savedNames);
  console.log('dasda');

  let nameIndex=0;

  if (savedOutputs) {
    for (let i = 0; i < savedOutputs.length; i++) {
      const newItem = document.createElement("button");
      const out = savedOutputs[i];
      console.log(out);
      newItem.textContent = savedNames[nameIndex];
      newItem.id = i;
      itemList.appendChild(newItem);
      const outp = output(savedOutputs[i].ip,savedOutputs[i].type+savedOutputs[i].pin)[savedOutputs[i].type+savedOutputs[i].pin];
      outputs.push(outp);

      // Add a click event listener to the new button, to toggle the output
      let clickCount = 0;
      newItem.addEventListener("click", function () {
        clickCount++;
        if (clickCount % 2 === 1) {
          console.log(savedOutputs[newItem.id],newItem.id);
          console.log(outputs);
          outputs[0].set("high");
        } else {
          outputs[0].set("low");
        }
      });
    }
  }
};

//clear  local storage  for testing
//localStorage.clear();

// Add a click event listener to the add-item button
addItemButton.addEventListener("click", handleAddItemClick);
