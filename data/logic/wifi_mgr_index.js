document
  .getElementById("wifi-form")
  .addEventListener("submit", function (event) {
    event.preventDefault();
    const cardGrid = document.querySelector(".card-grid");
    const ssid = document.getElementById("ssid").value;
    const password = document.getElementById("password").value;
    const popupDialog = document.getElementById("popup-dialog");
    const bottomNav = document.querySelector(".bottom-nav");
    if (
      ssid.length < 2 ||
      ssid.length > 32 ||
      password.length < 2 ||
      ssid.length > 32
    ) {
      console.log("Invalid credentials length");
      console.log("ssid length :" + ssid.length);
      console.log("pwd length -> " + password.length);
      bottomNav.style.color = "#AD1E05";
      bottomNav.textContent = "Yikes , Invalid credentials length.";
      return;
    }
    popupDialog.style.display = "block";
    cardGrid.style.display = "none";
    setTimeout(() => {
      popupDialog.style.display = "none";
      const data = {
        ssid: ssid,
        password: password,
      };
      fetch("/setup", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
      })
        .then((response) => response.text())
        .then((message) => {
          console.log(message);
          if (message.includes("saved")) {
            bottomNav.style.color = "#08AF23";
            bottomNav.textContent =
              "Your Device has been configured successfully";
          } else {
            bottomNav.style.color = "#AD1E05";
            bottomNav.textContent = "Yikes , something went wrong.";
          }
        })
        .catch((error) => {
          console.error("Error:", error);
          bottomNav.style.color = "#AD1E05";
          bottomNav.textContent = "Yikes , something went wrong. -> " + error;
        });
    }, 3000);
  });
