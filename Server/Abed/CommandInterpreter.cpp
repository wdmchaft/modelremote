//
//  CommandInterpreter.cpp
//  Abed
//
//  Created by Max Goedjen on 4/10/11.
//  UHD
//

#include "CommandInterpreter.h"

CommandInterpreter::CommandInterpreter()
{
    display = XOpenDisplay(NULL);
}

void CommandInterpreter::queryResolution()
{
	int numberSizes;
    Rotation originalRotation;
    
    XRRScreenSize *xrrs = XRRSizes(display, 0, &numberSizes);
    
    XRRScreenConfiguration *configuration = XRRGetScreenInfo(display, RootWindow(display, 0));
    SizeID originalSize = XRRConfigCurrentConfiguration(configuration, &originalRotation);
    
    int width=xrrs[originalSize].width;
    int height=xrrs[originalSize].height;
    
    xOrigin = (int)width / 4;
    yOrigin = (int)height / 4;
}

void CommandInterpreter::handleCommand(TACommand command)
{
    //printf("COMMAND: %i, %i, %i, %i\n", command.type, command.touch, command.xDifference, command.yDifference);
    switch (command.touch) 
	{
		case TACommandTouchEnd:
            releaseMouse(command);
            break;
        case TACommandTouchStart:
            click(command);
            break;
        case TACommandTouchMove:
            moveMouse(command);
            break;
        default:
            break;
	}
}

void CommandInterpreter::click(TACommand command)
{	
	XEvent event;
	memset(&event, 0x00, sizeof(&event));
	event.xbutton.button = Button1;
	event.xbutton.same_screen = True;
	
	if (XGrabPointer(display, RootWindow(display, DefaultScreen(display)), True, ButtonPressMask, GrabModeSync, GrabModeSync, RootWindow(display, DefaultScreen(display)), None, CurrentTime) == GrabNotViewable)
	{
		printf("Error on grabbing the pointer.");
	}
	
	//XAllowEvents(display, SyncBoth, CurrentTime);
	printf("Sending click.\n");
	XSendEvent(display, RootWindow(display, DefaultScreen(display)), True, ButtonPressMask, &event);
	
	XFlush(display);
}

void CommandInterpreter::releaseMouse(TACommand command)
{
	XEvent event;
	memset(&event, 0x00, sizeof(&event));
	event.xbutton.button = Button1;
	event.xbutton.same_screen = True;
	
	/*if (XGrabPointer(display, RootWindow(display, DefaultScreen(display)), True, ButtonReleaseMask, GrabModeSync, GrabModeSync, RootWindow(display, DefaultScreen(display)), None, CurrentTime) == GrabNotViewable)
	{
		printf("Error on grabbing the pointer.");
	}*/
	
	//XAllowEvents(display, SyncBoth, CurrentTime);
	printf("Sending release click.\n");
	XSendEvent(display, RootWindow(display, DefaultScreen(display)), True, ButtonReleaseMask, &event);
	printf("Releasing pointer\n");
	XUngrabPointer(display, CurrentTime);
	
	XFlush(display);
}

void CommandInterpreter::moveMouse(TACommand command)
{
    int absX = xOrigin + command.xDifference;
    int absY = yOrigin + command.yDifference;
    
	printf("Sending move command. X: %i Y: %i\n", absX, absY);
    XWarpPointer (display, None, RootWindow(display, 0), 0, 0, 0, 0, absX, absY);
    XFlush (display);
	usleep(100);
}