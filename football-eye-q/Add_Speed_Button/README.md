# Add_Speed_Button

This folder contains a ready-to-push project scaffold created in this environment. It is intended to hold the code for adding transient speed controls (e.g., 0.5×, 1.0×, 1.5×, 2.0×) to lighting patterns.

## What is included
- A placeholder source directory where you can add firmware or UI code.
- A Git ignore file tuned for Arduino/ESP32 and web assets.
- Instructions for pushing the project to a new GitHub repository once credentials are available.

## Project structure
- `src/` — add your application source here.
- `data/` — optional web assets if you plan to serve files from SPIFFS or LittleFS.
- `README.md` — this document.
- `.gitignore` — ignores Arduino build outputs, Node modules, and common editor files.

## How to push to GitHub later
1. Create an empty repository in your GitHub account named `Add_Speed_Button`.
2. In this environment, from the `Add_Speed_Button` folder, run:
   ```bash
   git init
   git add .
   git commit -m "chore: initial project scaffold"
   git remote add origin <your-new-repo-url>
   git branch -M main
   git push -u origin main
   ```
3. Provide GitHub credentials (SSH key or PAT) so the push can authenticate.

You can now build out the feature set in `src/` and `data/` before pushing.
