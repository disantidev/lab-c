1. Pipes (pipe)
The Challenge: Master parent-child data flow and file descriptor redirection.

Level 1: The Multi-Stage Filter: Create a program that forks two children. The first child runs cat on a file, pipes it to the second child which runs grep, which then pipes the final output back to the parent to be printed.

Level 2: The Parallel Math Engine: A parent process forks 4 children. The parent sends a large array of numbers through a pipe. Each child reads a portion, calculates the square root, and pipes the result back.

Level 3: The Mini-Shell: Write a program that parses a user string like ls -l | sort | uniq and manually sets up the pipe() and dup2() calls to make the execution work just like a real bash shell.

2. FIFOs (Named Pipes)
The Challenge: Handling unrelated processes and "rendezvous" points.

Level 1: The Centralized Logger: Create a background "Logger" daemon that reads from a FIFO at /tmp/my_log. Write a separate "Client" program. If you run 5 instances of the client at once, ensure the logger prints all their messages without interleaving lines.

Level 2: The Request-Response Bridge: Create two FIFOs: request_pipe and reply_pipe. A server reads a string from the request pipe, reverses it, and writes it to the reply pipe.

Level 3: The Chat Room: Build a primitive chat system where every user writes to a common GLOBAL_FIFO, and a "Broadcast Server" reads from it and sends the text to each user’s private individual FIFO.

3. Message Queues (System V or POSIX)
The Challenge: Working with structured, prioritized data.

Level 1: The Print Spooler: Create a system where "Job" messages have different priorities (1 for low, 10 for high). Ensure the consumer process always pulls the Priority 10 jobs off the queue first, even if they arrived later.

Level 2: The Heartbeat Monitor: A "Monitor" process expects a message from 3 different "Worker" processes every 2 seconds. If a worker fails to send its message to the queue, the Monitor declares that process "Dead."

Level 3: The Async File Uploader: A web-simulator process puts "File Paths" into a queue. A worker process picks them up and "uploads" them (simulated by a sleep and a log). This handles "bursty" traffic without crashing the main process.

4. Shared Memory & Semaphores
The Challenge: Zero-copy speed and the "Critical Section" problem.

Level 1: The Shared Counter: Create two processes that both try to increment a single integer in shared memory 1,000,000 times. Use a Semaphore to prove that without it, the final count is wrong (Race Condition), and with it, the count is exactly 2,000,000.

Level 2: The Video Frame Buffer: Process A "renders" a 2D ASCII art frame into shared memory. Process B "displays" it. Use semaphores to ensure Process B never starts drawing until Process A has finished the entire frame (Double Buffering logic).

Level 3: The Multi-Process Hash Table: Implement a basic Hash Table in a shared memory segment. Multiple unrelated processes must be able to insert() and lookup() keys simultaneously using semaphores to lock specific "buckets" of the table.

5. Unix Domain Sockets
The Challenge: High-speed local networking and descriptor passing.

Level 1: The Local Echo Server: Build a server that listens on a socket file (e.g., /tmp/echo.sock). Use nc -U /tmp/echo.sock to test sending strings to it.

Level 2: The Performance Race: Write a program that transfers 1GB of data over localhost:8080 (TCP) and then over a Unix Domain Socket. Measure the time difference to see the "Kernel Bypass" speed boost.

Level 3: The Privilege Separator (Advanced): Create a "Root" process that opens a protected file (like /etc/shadow) and "passes" the File Descriptor over a Unix Socket to a "Non-Root" process that is otherwise forbidden from opening it.
