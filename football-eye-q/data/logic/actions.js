// Get all page elements
const sessionPage = document.getElementById("session-page");
const settingsPage = document.getElementById("settings-page");
const patternsPage = document.getElementById("patterns-page");
const reprovBanner = document.getElementById("reprov-banner");
const tableContainer = document.getElementById("table-container");
const speedButtons = document.querySelectorAll(".speed-button");

// Get all buttons
const patternsBtn = document.getElementById("patterns-btn");
const settingsBtn = document.getElementById("settings-btn");
const sessionSubmitBtn = document.getElementById("session-submit-btn");
const newSessionBtn = document.getElementById("new-session-btn");

const testNetworkToggle = document.getElementById("test-network-toggle");


// Get session input elements
const sessionCodeInput = document.getElementById("session-code-input");
const sessionErrorText = document.getElementById("session-error");

// --- Session code decoding helpers ---
const BASE36_ALPHABET = "0123456789abcdefghijklmnopqrstuvwxyz";
const BASE71_POWERS = [25411681, 357911, 5041, 71, 1]; // 71^4 .. 71^0 (most significant first)

function isBase36SessionCode(code) {
  return code.length === 6 && /^[0-9a-z]{6}$/.test(code);
}

function decodeBase36SessionCode(code) {
  let value = 0;
  for (let i = 0; i < code.length; i++) {
    const digit = BASE36_ALPHABET.indexOf(code[i]);
    if (digit < 0) {
      return { success: false, error: "Error: Code must use 0-9 or a-z." };
    }
    value = value * 36 + digit;
  }

  const digits = [];
  let remaining = value;
  for (let i = 0; i < BASE71_POWERS.length; i++) {
    const divisor = BASE71_POWERS[i];
    const di = Math.floor(remaining / divisor); // extract most significant first
    remaining -= di * divisor;
    if (di < 0 || di > 70) {
      return { success: false, error: "Error: Invalid encoded session." };
    }
    digits.push(di);
  }

  while (digits.length && digits[digits.length - 1] === 0) {
    digits.pop();
  }

  if (digits.length === 0) {
    return { success: false, error: "Error: Session code decodes to empty playlist." };
  }

  if (digits.some((d) => d === 0)) {
    return { success: false, error: "Error: Session code contains invalid zero." };
  }

  const decodedString = digits.map((d) => d.toString().padStart(2, "0")).join("");
  return { success: true, decodedString };
}

function isValidDecodedPatternString(code) {
  return code.length >= 2 && code.length <= 10 && code.length % 2 === 0 && /^\d+$/.test(code);
}

const table = document.createElement("table");
let activePattern = null;
let selectedSpeedMultiplier = 1.0;

const DEFAULT_SPEED_MULTIPLIER = 1.0;

function setSelectedSpeedMultiplier(multiplier) {
  selectedSpeedMultiplier = multiplier;
  speedButtons.forEach((btn) => {
    const matchesSelected = parseFloat(btn.dataset.speed) === multiplier;
    btn.classList.toggle("active", matchesSelected);
  });
}

function resetSpeedSelection() {
  setSelectedSpeedMultiplier(DEFAULT_SPEED_MULTIPLIER);
}

speedButtons.forEach((button) => {
  button.addEventListener("click", () => {
    const newMultiplier = parseFloat(button.dataset.speed);
    setSelectedSpeedMultiplier(newMultiplier);

    if (activePattern !== null) {
      const activeSlider = activePattern === 99
        ? testNetworkToggle
        : document.getElementById(`pattern${activePattern}`);
      if (activeSlider) {
        handleSliderToggleChange(activeSlider, activePattern, true);
      }
    }
  });
});

resetSpeedSelection();

// --- Event Listeners ---

patternsBtn.addEventListener("click", onPatternsPressed);
settingsBtn.addEventListener("click", onSettingsPressed);
sessionSubmitBtn.addEventListener("click", onSessionSubmit);
newSessionBtn.addEventListener("click", onNewSession);

testNetworkToggle.addEventListener("click", () => handleSliderToggleChange(testNetworkToggle, 99, undefined));


// --- Initialization ---

function initIndexPage() {
  console.log("Loading page initially.");
  // Show the session page by default, hide the others
  sessionPage.style.display = "block";
  patternsPage.style.display = "none";
  settingsPage.style.display = "none";
  reprovBanner.style.display = "none";

  // Hide the nav buttons until a session is loaded
  patternsBtn.style.display = "none";
  settingsBtn.style.display = "none";
}

// --- Page Navigation ---

function showPage(pageToShow) {
  // Hide all main pages
  sessionPage.style.display = "none";
  patternsPage.style.display = "none";
  settingsPage.style.display = "none";
  
  // Show the requested one
  pageToShow.style.display = "block";
}

function onPatternsPressed() {
  showPage(patternsPage);
  console.log("Show Playlist Page");
}

function onSettingsPressed() {
  showPage(settingsPage);
  console.log("Show Settings Page");
}

function onNewSession() {
  // Stop any active pattern
  if (activePattern !== null) {
      // Find the correct slider to turn off
      const activeSlider = (activePattern === 99) 
                            ? testNetworkToggle 
                            : document.getElementById(`pattern${activePattern}`);
      if (activeSlider) {
          handleSliderToggleChange(activeSlider, activePattern, false); // Force OFF
      }
  }
  
  // Clear the table
  while (table.rows.length > 0) {
    table.deleteRow(0);
  }
  
  // Show session entry, hide nav buttons
  showPage(sessionPage);
  patternsBtn.style.display = "none";
  settingsBtn.style.display = "none";
  sessionCodeInput.value = "";
  sessionErrorText.textContent = "";
}

// --- Core Session Logic ---

function onSessionSubmit() {
  const rawInput = sessionCodeInput.value.trim();
  const code = rawInput.toLowerCase();

  if (testNetworkToggle.checked) {
      sessionErrorText.textContent = "Please stop the Network Test first.";
      return;
  }

  let decodedString = "";

  if (isBase36SessionCode(code)) {
    const decoded = decodeBase36SessionCode(code);
    if (!decoded.success) {
      sessionErrorText.textContent = decoded.error;
      return;
    }
    decodedString = decoded.decodedString;
  } else {
    if (!isValidDecodedPatternString(code)) {
      sessionErrorText.textContent = "Error: Code must be 2, 4, 6, 8, or 10 digits.";
      return;
    }
    decodedString = code;
  }

  sessionErrorText.textContent = "";

  const patternIds = decodedString.match(/.{1,2}/g) || [];
  const patternNumbers = patternIds.map((id) => parseInt(id, 10));

  if (patternNumbers.some((id) => id === 0)) {
    sessionErrorText.textContent = "Error: Session code contains invalid pattern 00.";
    return;
  }

  // Build the table with these patterns
  createPatternsTable(patternNumbers);

  // Show the patterns page and nav buttons
  showPage(patternsPage);
  patternsBtn.style.display = "block";
  settingsBtn.style.display = "block";
}

// --- Table and Pattern Logic ---

function createPatternsTable(patternNumbers) {
  // Clear old table content
  while (table.rows.length > 0) {
    table.deleteRow(0);
  }
  table.style.width = "100%";
  table.classList.add("patterns-page");

  // Create a row for each pattern ID
  patternNumbers.forEach(patternId => {
    // Discard IDs outside 1-70 (base-71 encoded range)
    if (patternId < 1 || patternId > 70) {
        console.warn(`Invalid pattern ID ${patternId} discarded.`);
        return;
    }
    const newRow = createRow(patternId);
    table.appendChild(newRow);
  });
  
  if (table.rows.length === 0) {
    // All IDs were invalid
    onNewSession(); // Go back to session page
    sessionErrorText.textContent = "Error: Session code contains no valid patterns (1-70).";
    return;
  }

  tableContainer.appendChild(table);
}

function createRow(rowNumber) {
  const row = document.createElement("tr");
  row.appendChild(createCell("Pattern " + rowNumber));
  const cell = createCell("");
  cell.appendChild(createSliderToggleSwitch(rowNumber));
  row.appendChild(cell);
  return row;
}

function createCell(text) {
  const cell = document.createElement("td");
  cell.textContent = text;
  return cell;
}

function createSliderToggleSwitch(rowNumber) {
  const slider = document.createElement("input");
  slider.type = "checkbox";
  slider.className = "slider-toggle";
  slider.id = `pattern${rowNumber}`;
  
  slider.addEventListener("click", () => handleSliderToggleChange(slider, rowNumber, undefined));
  return slider;
}

function handleSliderToggleChange(slider, rowNumber, forceState) {
  const isChecked = (forceState !== undefined) ? forceState : slider.checked;

  const isStartingPattern = isChecked && activePattern !== rowNumber;

  slider.checked = isChecked;

  if (isStartingPattern || !isChecked) {
    resetSpeedSelection();
  }

  if (isChecked) {
    // A pattern is turned ON
    // If another pattern is already active, turn it OFF first
    if (activePattern !== null && activePattern !== rowNumber) {
      // Check if the other pattern is a playlist pattern
      const previousActiveSlider = (activePattern === 99)
                                    ? testNetworkToggle
                                    : document.getElementById(`pattern${activePattern}`);
      if (previousActiveSlider) {
          handleSliderToggleChange(previousActiveSlider, activePattern, false); // Force OFF
      }
    }
    // Set the new active pattern
    activePattern = rowNumber;
    console.log(`Pattern ${rowNumber} is ON`);

  } else {
    // A pattern is turned OFF
    if (activePattern === rowNumber) {
      activePattern = null; 
    }
    console.log(`Pattern ${rowNumber} is OFF`);
  }
  
  // Send the command to the ESP32 backend
  const data = {
    pattern: rowNumber,
    state: isChecked,
    speed: selectedSpeedMultiplier,
  };
  fetch("/patterns", {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify(data),
  })
    .then((response) => response.text())
    .then((message) => {
      console.log(message);
    })
    .catch((error) => {
      console.error("Error:", error);
    });
}


// --- Settings Page Logic (Unchanged) ---

document.addEventListener("DOMContentLoaded", function () {
  const tabs = document.querySelectorAll(".tab");
  const tabContents = document.querySelectorAll(".tab-content");

  tabs.forEach((tab) => {
    tab.addEventListener("click", function () {
      tabs.forEach((t) => t.classList.remove("active"));
      tab.classList.add("active");

      tabContents.forEach((content) => (content.style.display = "none"));
      const targetContentId = tab
        .getAttribute("id")
        .replace("-tab", "-content");
      const targetContent = document.getElementById(targetContentId);
      targetContent.style.display = "block";
    });
  });
});

document
  .getElementById("reprovision-button")
  .addEventListener("click", function () {
    var confirmation = confirm(
      "Are you sure you want to reprovision your device ?"
    );
    if (confirmation) {
      alert("Your device will be restarted in WiFi Manaager Mode.");
      settingsPage.style.display = "none";
      patternsPage.style.display = "none";
      patternsBtn.style.display = "none";
      settingsBtn.style.display = "none";
      reprovBanner.style.display = "block";
      const data = {
        reprov: "reprovision",
      };
      fetch("/reprovision", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
      })
        .then((response) => response.text())
        .then((message) => {
          console.log(message);
        })
        .catch((error) => {
          console.error("Error:", error);
        });
    } else {
      alert("Reprovisioning your device has been cancelled.");
    }
  });


// --- START THE APP ---
function runSessionDecoderSelfTest() {
  const samples = [
    { code: "0fkc03", expected: "0102030405" },
    { code: "4bn9sk", expected: "102030" },
    { code: "aoqfku", expected: "253035" },
  ];

  samples.forEach((sample) => {
    const decoded = decodeBase36SessionCode(sample.code);
    if (!decoded.success || decoded.decodedString !== sample.expected) {
      console.warn(`Session decode self-test failed for ${sample.code}`);
    }
  });
}

runSessionDecoderSelfTest();
initIndexPage();