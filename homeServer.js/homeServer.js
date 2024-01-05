import {output} from '../lib.js/script.js';

// Get the add-item button element
const addItemButton = document.getElementById('add');

// Get the list element
const itemList = document.getElementById('item-list');

// Get the input element
const inputField = document.getElementById('new-item');

// Create an array to store the digital outputs
let outputs = [];

// Function to handle the add-item button click event
function handleAddItemClick() {
    // Get the value of the input field
    const newItemText = inputField.value;
    
    // Get the index of the first '/' character
    const slashIndex = newItemText.indexOf('/');
    
    // Get the text before the '/'
    const beginningText = newItemText.substring(0, slashIndex);

    // Clear the input field
    inputField.value = '';

    // Create a new button element
    const newItem = document.createElement('button');
    
    // Set the text of the button
    newItem.textContent = newItemText.substring(slashIndex+1, newItemText.length);

    newItem.id = outputs.length;

    // Add the button to the list
    itemList.appendChild(newItem);

    // Add the output to the outputs array
    outputs.push(output("192.168.1.138", beginningText)[beginningText]) - 1;

    // Add a click event listener to the new button, to toggle the output
    let clickCount = 0;
    newItem.addEventListener('click', function() {
        clickCount++;
        if (clickCount % 2 === 1) {
            outputs[newItem.id].set("high");
        } else {
            outputs[newItem.id].set("low");
        }
    });
}

// Add a click event listener to the add-item button
addItemButton.addEventListener('click', handleAddItemClick);

