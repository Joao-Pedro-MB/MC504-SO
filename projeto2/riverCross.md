// boat has exactly 4 spaces to go and all need to be occupied
// there are serfs and hackers
// a hacker cant go with 3 serfs
// a serfs cant go with 3 hackers

// each thread is a serf or hacker
// when boarding the boat a function called board must be evoked
// we must be sure that all 4 passangers called board function before the next boatload
// after all invoked board exactly one of them should call rowBoat funtion
// traffic goes on only one direction

mutex.wait()
    hackers += 1
    if hackers == 4:
        hackerQueue . signal (4)
        hackers = 0
        isCaptain = True
    elif hackers == 2 and serfs >= 2:
        hackerQueue.signal(2)
        serfQueue.signal(2)
        serfs -= 2
        hackers = 0
        isCaptain = True
    else:
        mutex.signal() // captain keeps the mutex

hackerQueue.wait ()

board()
barrier.wait()

if isCaptain :
    rowBoat()
    mutex.signal() // captain releases the mutex