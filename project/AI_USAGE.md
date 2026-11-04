# AI Usage – Washing Machine Project

**Course:** ECE 361  
**Project:** Project 8 – Washing Machine

## AI Tools Used

For this project, I used ChatGPT as a learning tool to help me understand the assignment, review programming concepts, and figure out how to test my work.

## How AI Helped Me Understand the Project

- **Understanding the assignment:** ChatGPT helped me break down the instructions into smaller steps. This made it easier for me to understand what I needed to work on and how the different parts of the project connected.

- **Finite State Machine (FSM):** I used ChatGPT to better understand how the washing machine moves between its different states. It helped me learn what events cause the machine to change states and how the transition table works.

- **Binary Search Tree (BST):** ChatGPT helped explain how the binary search tree stores records using the washing machine state and timestamp. I also learned how an in-order traversal organizes the records.

- **Event Stack:** I used ChatGPT to understand how the stack stores the history of state changes. It helped me understand last-in, first-out (LIFO) and why the stack needs to increase its size when it becomes full.

- **Sensors and Bit Fields:** ChatGPT helped me understand how temperature and humidity values are converted from sensor readings. I also reviewed how different pieces of information can be stored inside a 32-bit status word.

- **Testing:** One of the biggest things ChatGPT helped me understand was how to test my program. I learned how to use `make`, `make test`, and `make clean`, and how to read the terminal results to check whether my program was working correctly.

- **GitHub:** ChatGPT also helped me understand how to organize my files, commit my changes, and push them to GitHub. I learned why `.gitignore` is useful for keeping unnecessary compiled files out of the repository.

## Something AI Got Wrong

One thing that needed to be corrected was how the scenario tests were checked in the Makefile. At first, the tests ran without comparing most of the results to their expected outputs.

After reviewing the assignment requirements, I learned that running a program successfully does not always mean the output is correct. I updated the testing process to compare the actual results with the expected results using `diff -u`.

After making that change, I ran the tests again and got 303 passing unit tests and eight successful scenario comparisons.

This helped me understand the importance of checking the results instead of only checking whether the program runs.

## What I Learned

Overall, ChatGPT was useful because it helped me understand programming concepts that I was still learning in class. Being able to ask questions and get simpler explanations made it easier for me to follow the assignment.

I learned more about state machines, binary search trees, stacks, sensors, and testing. I also became more comfortable using the terminal and GitHub.

The biggest thing I learned was that I should always review the code, check the assignment instructions, and test my work instead of assuming that every suggestion from AI is correct.