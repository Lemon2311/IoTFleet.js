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

function switchDX(x,state) {
  fetch('http://192.168.1.243/d'+x+'/'+state)
    .then(response => {
      if (!response.ok) {
        throw new Error('Network response was not ok');
      }
      return response.text(); // You can change this to response.json() if the server returns JSON.
    })
    .then(data => {
      console.log('Response:', data);
      // You can add further handling here if needed.
    })
    .catch(error => {
      console.error('Error:', error);
      // Handle errors here, e.g., display an error message to the user.
    });

    return state
}

switchDX(13,"off");
