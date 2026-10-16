# Virtual Memory Page Replacement Visualizer

## Project Overview

The **Virtual Memory Page Replacement Visualizer** is a web-based application developed to demonstrate and compare three page replacement algorithms: FIFO (First-In, First-Out), LRU (Least Recently Used), and Optimal Page Replacement.

The application helps students understand how operating systems manage memory when the required page is not available in the allocated memory frames. It visually displays page hits, page faults, frame changes, and the performance of each algorithm.

## Objectives

- Understand virtual memory and page replacement.
- Simulate FIFO, LRU, and Optimal algorithms.
- Display memory frame changes step by step.
- Identify page hits and page faults.
- Calculate and compare total page faults.
- Provide an interactive learning experience for students.

## Features

- Enter a custom page reference string.
- Set the number of available memory frames.
- Simulate FIFO, LRU, and Optimal algorithms.
- View step-by-step memory frame updates.
- Identify page hits and page faults.
- Display total page faults and hit rates.
- Compare algorithm performance using a chart.
- Use sample inputs for quick testing.
- Access the application through a responsive web interface.

## Technologies Used

- **HTML5** – Structures the web application.
- **CSS3** – Provides styling and responsive design.
- **JavaScript** – Implements the page replacement algorithms and simulation logic.
- **HTML Canvas** – Displays the performance comparison chart.

## Algorithms Implemented

### 1. FIFO (First-In, First-Out)
FIFO replaces the page that entered memory first. It follows the order in which pages were loaded into the frames.

### 2. LRU (Least Recently Used)
LRU replaces the page that has not been used for the longest period of time.

### 3. Optimal Page Replacement
Optimal replaces the page that will not be used for the longest time in the future. It is useful as a theoretical benchmark for comparing page replacement performance.

## How to Run the Project

1. Download or clone the project files.
2. Extract the ZIP file if downloaded.
3. Open the project folder in Visual Studio Code.
4. Locate `index.html`.
5. Open `index.html` in a web browser, or use the **Live Server** extension in Visual Studio Code.
6. Enter a page reference string and specify the number of memory frames.
7. Run the simulation to view and compare the results.

No additional software installation or database setup is required.

## Sample Input

**Page Reference String:**
`1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5`

**Number of Frames:**
`3`

The application processes the same input using FIFO, LRU, and Optimal algorithms and displays the resulting page faults and hit rates.

## Expected Output

The application displays:

- Step-by-step memory frame contents.
- Page hit and page fault status for each reference.
- Total page faults for each algorithm.
- Page hit rates.
- A visual comparison of algorithm performance.

The number of page faults depends on the page reference string and the number of available frames.

## Advantages

- Makes virtual memory concepts easier to understand.
- Provides a visual comparison of page replacement algorithms.
- Reduces the need for manual calculations.
- Supports interactive learning and experimentation.
- Helps students understand the relationship between memory frames and page faults.

## Future Scope

- Add more algorithms, such as LFU (Least Frequently Used).
- Introduce animations and step-by-step playback controls.
- Provide downloadable simulation reports.
- Add interactive quizzes and learning materials.
- Support larger inputs and additional performance statistics.

## Conclusion

The Virtual Memory Page Replacement Visualizer is an educational tool that simplifies the study of virtual memory management. By simulating FIFO, LRU, and Optimal algorithms, it helps users observe memory frame changes, understand page hits and page faults, and compare algorithm performance through an interactive web interface.

## Project Information

**Project Name:** Virtual Memory Page Replacement Visualizer  
**Domain:** Operating Systems and Systems Programming  
**Project Type:** Web-Based Simulation  
**Technologies:** HTML, CSS, JavaScript

---

*Developed as an educational project to demonstrate virtual memory and page replacement algorithms.*
