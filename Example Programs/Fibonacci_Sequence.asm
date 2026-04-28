RESET:
    // Initiate F(n-2)
    DATA Rb, #0
    // Initiate F(n-1)
    DATA Rc, #1 

    // Output Initial Values
    OUT Rb
    OUT Rc

LOOP:
    // Add New Fibonacci Number
    MOV Ra, Rc
    ADD Rb

    // Jump If Too Large
    JC RESET

    // Rb = F(n-1)
    MOV Rb, Rc
    // Rc = New Fibonacci
    MOV Rc, Ra

    // Output Result
    OUT Ra

    JMP LOOP
