# Code Simulation for Arduino with NewPing
## Instructions for simulation using WOKWI
1. Go to [WOKWI](https://wokwi.com/dashboard/projects).
2. Create NEW PROJECT.
3. Select Arduino Uno.
4. Copy and paste your code to sketch.ino in the browser.
5. Click on diagram.json in the tabs.
6. Copy and paste the content of [diagram.json in our respository](https://github.com/shihlings/ubc-apsc101/tree/main/simulation_use/diagram.json) to the diagram.json in the browser. The correct circuit should appear on the right of the browser window.
7. Click on Library Manager in the browser.
8. Click on the (+) menu in the Project Libraries plane (not the one in the Arduino plane).
9. Search for NewPing and add that to the library.
10-1. Confirm that both "Servo" and "NewPing" are present in the Project Libraries plane.
10-2. If "Servo" is not present. You will have to add it manually. Go back to step 7 - 9. This time, search for Servo instead of NewPing. There are many Servo libraries, but find the one that contains only Servo.
11. The simulation environment is all set up. Click on the (+) in the Arduino plane to start the simulation.
