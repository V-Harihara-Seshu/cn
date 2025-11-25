# Create a new Simulator instance
set ns [new Simulator]

# Open files to store NAM and trace output
set nf [open prog1.nam w]
$ns namtrace-all $nf

set nd [open prog1.tr w]
$ns trace-all $nd

# Define a procedure to end the simulation and close the trace files
proc finish {} {
    global ns nf nd
    $ns flush-trace
    close $nf
    close $nd
    exec nam prog1.nam &
    exit 0
}

# Create network nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]

# Create duplex links between nodes with bandwidth, delay, queue
$ns duplex-link $n0 $n1 1Mb 10ms DropTail
$ns duplex-link $n1 $n2 512kb 10ms DropTail

# Set queue limit on link between n1 and n2
$ns queue-limit $n1 $n2 10

# Attach a UDP agent to node n0
set udp0 [new Agent/UDP]
$ns attach-agent $n0 $udp0

# Create a CBR (Constant Bit Rate) traffic generator
set cbr0 [new Application/Traffic/CBR]
$cbr0 set packetSize_ 500
$cbr0 set interval_ 0.005
$cbr0 attach-agent $udp0

# Attach NULL agent (sink) to node n2
set sink [new Agent/Null]
$ns attach-agent $n2 $sink

# Connect UDP agent on node n0 to sink on node n2
$ns connect $udp0 $sink

# Schedule simulation events
$ns at 0.2 "$cbr0 start"
$ns at 4.5 "$cbr0 stop"
$ns at 5.0 "finish"

# Run the simulation
$ns run


--------------------------------------
awk file


BEGIN {
    dcount = 0;
    rcount = 0;
}

{
    event = $1;
    if (event == "d") {
        dcount++;
    }
    if (event == "r") {
        rcount++;
    }
}

END {
    printf("The no. of packets dropped: %d\n", dcount);
    printf("The no. of packets received: %d\n", rcount);
}
