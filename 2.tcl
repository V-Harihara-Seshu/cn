# Create a new instance of the NS2 simulator
set ns [new Simulator]

# Open the files for writing the NAM and trace outputs
set nf [open prog2.nam w]
$ns namtrace-all $nf

set nd [open prog2.tr w]
$ns trace-all $nd

# Define the finish procedure to end the simulation
proc finish {} {
    global ns nf nd
    $ns flush-trace
    close $nf
    close $nd
    exec nam prog2.nam &
    exit 0
}

# Create network nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]

# Establish duplex links between nodes and n0
$ns duplex-link $n1 $n0 1Mb 10ms DropTail
$ns duplex-link $n2 $n0 1Mb 10ms DropTail
$ns duplex-link $n3 $n0 1Mb 10ms DropTail
$ns duplex-link $n4 $n0 1Mb 10ms DropTail
$ns duplex-link $n5 $n0 1Mb 10ms DropTail
$ns duplex-link $n6 $n0 1Mb 10ms DropTail

# Create a custom procedure for Ping agent to display RTT
Agent/Ping instproc recv {from rtt} {
    $self instvar node_
    puts "node [$node_ id] received ping reply from $from with round-trip-time $rtt ms."
}

# Create Ping agents
set p1 [new Agent/Ping]
set p2 [new Agent/Ping]
set p3 [new Agent/Ping]
set p4 [new Agent/Ping]
set p5 [new Agent/Ping]
set p6 [new Agent/Ping]

# Attach Ping agents to nodes
$ns attach-agent $n1 $p1
$ns attach-agent $n2 $p2
$ns attach-agent $n3 $p3
$ns attach-agent $n4 $p4
$ns attach-agent $n5 $p5
$ns attach-agent $n6 $p6

# Set queue limits
$ns queue-limit $n0 $n4 3
$ns queue-limit $n0 $n5 2
$ns queue-limit $n0 $n6 2

# Connect Ping agents
$ns connect $p1 $p4
$ns connect $p2 $p5
$ns connect $p3 $p6

# Schedule Ping transmissions
$ns at 0.2 "$p1 send"
$ns at 0.4 "$p2 send"
$ns at 0.6 "$p3 send"
$ns at 1.0 "$p4 send"
$ns at 1.2 "$p5 send"
$ns at 1.4 "$p6 send"

# End the simulation
$ns at 2.0 "finish"

# Run the simulation
$ns run


----------------
awk file

BEGIN {
    count = 0;
}

{
    event = $1;
    if (event == "d") {
        count++;
    }
}

END {
    printf("No of packets dropped: %d\n", count);
}
