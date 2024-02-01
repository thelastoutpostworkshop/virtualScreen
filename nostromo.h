#include "virtualScreen.h"

const int charWidth = 6;   // Width of each character
const int charHeight = 8;  // Height of each character
const uint16_t textColor = TFT_GREEN;  // Text color
const uint16_t bgColor = TFT_BLACK;    // Background color

// Function to shift the display content upwards
void scrollUp(VirtualDisplay *display, int scrollHeight) {
    int moveAreaHeight = display->height() - scrollHeight;
    uint16_t *buffer = new uint16_t[display->width() * moveAreaHeight];
    display->readRect(0, scrollHeight, display->width(), moveAreaHeight, buffer);
    display->pushImage(0, 0, display->width(), moveAreaHeight, buffer);

    display->fillRect(0, display->height() - scrollHeight, display->width(), scrollHeight, bgColor);

    // Free the allocated buffer
    delete[] buffer;
}


// Function to print a message at the bottom of the screen
void printMessage(VirtualDisplay *display, const String &message) {
    int bottomLineY = display->height() - charHeight; // Y position of the bottom line

    // Scroll up the existing content to make space for the new message
    scrollUp(display, charHeight);

    // Print the new message at the bottom
    display->setCursor(0, bottomLineY);
    display->setTextColor(textColor);
    
    for (char c : message) {
        display->print(c);
        display->output();  // Update the display
    }
}

// Function to run the Nostromo simulation
void runNostromoSimulation(VirtualDisplay *display) {
    display->fillScreen(bgColor);

    while (true) {
        printMessage(display, "NOSTROMO SYSTEM READY");

        // Add more messages as needed
    }
}