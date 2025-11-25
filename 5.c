#include <stdio.h>
#include <unistd.h> // for usleep()
#define MAX 100

// Sender structure
typedef struct {
    int windowSize;
    int window[MAX];
    int front, rear;
    int nextSeqNum;
    int ackNum;
} Sender;

// Initialize sender
void initSender(Sender *s, int windowSize) {
    s->windowSize = windowSize;
    s->front = 0;
    s->rear = -1;
    s->nextSeqNum = 0;
    s->ackNum = 0;
}

// Enqueue packet
void enqueue(Sender *s, int seq) {
    if (s->rear < MAX - 1) {
        s->rear++;
        s->window[s->rear] = seq;
    }
}

// Dequeue packet
void dequeue(Sender *s) {
    if (s->front <= s->rear) {
        s->front++;
    }
}

// Peek front of queue
int peek(Sender *s) {
    if (s->front <= s->rear)
        return s->window[s->front];
    return -1;
}

// Send packets within window
void sendPacket(Sender *s) {
    while (s->nextSeqNum < s->ackNum + s->windowSize) {
        printf("Sending packet: %d\n", s->nextSeqNum);
        enqueue(s, s->nextSeqNum);
        s->nextSeqNum++;
        usleep(500000); // simulate delay (0.5 sec)
    }
}

// Receive ACK
void receiveAck(Sender *s, int ack) {
    if (ack > s->ackNum && ack < s->nextSeqNum) {
        printf("Received ACK for packet: %d\n", ack);
        s->ackNum = ack;

        // Remove acknowledged packets
        while (s->front <= s->rear && peek(s) <= s->ackNum) {
            dequeue(s);
        }

        // Send next packets
        sendPacket(s);

    } else {
        printf("Invalid ACK: %d\n", ack);
    }
}

// Receiver simulation (manual ACK input)
void simulateReceiver(Sender *s) {
    int ack;
    while (1) {
        printf("Enter ACK number (or -1 to exit): ");
        scanf("%d", &ack);

        if (ack == -1) {
            printf("Exiting...\n");
            break;
        }

        receiveAck(s, ack);
    }
}

int main() {
    int windowSize = 3; // Change window size if needed
    Sender sender;

    initSender(&sender, windowSize);

    // Start sending packets
    sendPacket(&sender);

    // Receiver interaction
    simulateReceiver(&sender);

    return 0;
}
