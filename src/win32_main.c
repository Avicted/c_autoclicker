#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <windows.h>

// Defines
#define internal static // Internal for the translation unit

internal bool Running = true;
internal unsigned int ClickRateHz = 250;
internal bool PrevScrollLockState = false; // Variable to store the previous state of Scroll Lock

internal void
SimulateMouseClick(void)
{
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}

internal void
handle_input_args(int argc, char **argv)
{
    for (int i = 0; i < argc; ++i)
    {
        printf("\targv[%d]: %s\n", i, argv[i]);
    }
}

internal bool
IsScrollLockToggled(void)
{
    bool IsToggled = (GetKeyState(VK_SCROLL) & 0x0001) != 0;

    // Print message only when there is a change in the state
    if (IsToggled != PrevScrollLockState)
    {
        printf("\tAutoclicker: %s\n", IsToggled ? "enabled" : "disabled");
        PrevScrollLockState = IsToggled;
    }

    return IsToggled;
}

int main(int argc, char **argv)
{
    handle_input_args(argc, argv);

    printf("\tHello Sailor!\n");

    // Calculate time between clicks based on click rate
    unsigned int clickIntervalMs = 1000 / ClickRateHz;

    while (Running)
    {
        if (IsScrollLockToggled())
        {
            SimulateMouseClick();
        }

        Sleep(clickIntervalMs);
    }

    return (0);
}
