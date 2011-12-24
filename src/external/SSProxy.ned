package scadasim.external;

simple SSProxy
{
    parameters:
       string extAddress;
//        double aDelay;
//        volatile double serviceTime @unit(s);
//        volatile double sendIaTime @unit(s);
        @display("i=device/server2");
    gates:
        input in;
        output out;
}
