package scadasim.networklayer.queue;

simple DropTailTraceQueue
{
    parameters:
        int frameCapacity;
        double intervalSize @unit(s);
        bool traceOn;
    gates:
        input in;
        output out;
}



