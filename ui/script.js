// document.addEventListener("DOMContentLoaded", function () {
//     const items = document.querySelectorAll(".list-item");

//     items.forEach(item => {
//         const header = item.querySelector(".item-header");
//         const content = item.querySelector(".item-content");

//         header.addEventListener("click", () => {
//             if (content.style.maxHeight) {
//                 content.style.maxHeight = null;
//             } else {
//                 content.style.maxHeight = content.scrollHeight + "px";
//             }
//         });
//     });
// });

//Function to change the state of a digital or analog output
function changePinState(pin, state) {
  const url = `http://192.168.1.138/digitalOutput?pin=${pin}&state=${state}`;

  return fetch(url, {
    method: 'POST', // Use POST method to send data
  })
    .then((response) => {
      if (!response.ok) {
        throw new Error('Network response was not ok');
      }
      return response.text(); // You can change this to response.json() if the server returns JSON.
    })
    .then((data) => {
      console.log('Response:', data);
      // You can add further handling here if needed.
    })
    .catch((error) => {
      console.error('Error:', error);
      // Handle errors here, e.g., display an error message to the user.
    });
}

// Function to initialize the pin using the initializeDigitalPin HTTP endpoint
async function initializePin(pin, mode) {
  const url = `http://192.168.1.138/initializeDigitalPin?pin=${pin}&mode=${mode}`;

  const response = await fetch(url, {
    method: 'POST',
  });

  if (!response.ok) {
    throw new Error('Failed to initialize pin');
  }
}

async function addItem() {
  const typeInput = document.querySelector("#addItemSection .type");
  const pinInput = document.querySelector(
    "#addItemSection input[type='text'][placeholder='pin']"
  );

  const type = typeInput.value.trim();
  const pin = pinInput.value.trim();

  if (type === "" || pin === "") {
    alert("Please enter valid values for 'digital/analog' and 'pin'.");
    return;
  }

  const itemList = document.getElementById("item-list");
  const listItem = document.createElement("li");
  listItem.className = "thin-list-item thin-list-item";
  listItem.innerHTML = `
      <img src="IO.svg" class="icon" />
      <span>${type}</span>
      <span>${pin}</span>
      <img src="false.svg" class="checkmark-icon toggle-icon" onclick="toggleIcon(this)" /> <!-- Include the checkmark here -->
  `;

  itemList.appendChild(listItem);

  try {
    await initializePin(pin, 'output');
    await changePinState(pin, 'low'); // Now this will wait for initializePin to complete
  } catch (error) {
    console.error(error);
    alert("There was an error initializing or changing the pin state.");
  }

  // Clear input fields
  typeInput.value = "";
  pinInput.value = "";
}

function toggleIcon(imgElement) {
  // Find the parent list item of the clicked image
  // This is necessary to get the context in which the image was clicked
  const listItem = imgElement.closest('.thin-list-item');

  // Extract the type and pin values from the list item
  // These values are assumed to be in the first and second span elements respectively
  // 'textContent' is used to get the text inside the span elements
  const type = listItem.querySelector('span:nth-of-type(1)').textContent;
  const pin = listItem.querySelector('span:nth-of-type(2)').textContent;

  // Initialize a variable to hold the new state of the pin
  let newState;

  // Check the current source of the image to determine the toggle action
  // If the current image is 'false.svg', switch to 'true.svg' and set the new state to 'on'
  // If the current image is not 'false.svg', switch to 'false.svg' and set the new state to 'off'
  if (imgElement.src.includes('false.svg')) {
      imgElement.src = 'true.svg';
      newState = 'high';
  } else {
      imgElement.src = 'false.svg';
      newState = 'low';
  }

  // Call the changePinState function with the type, pin, and the new state
  // This function is expected to handle the logic of changing the state of the pin
  // The exact behavior of this function depends on its implementation which is not shown here
  changePinState(pin, newState);
}

