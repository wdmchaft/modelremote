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
	queryResolution();
	currentEvent = NULL;
}

void CommandInterpreter::queryResolution()
{
	int numberSizes;
    Rotation originalRotation;
    
    XRRScreenSize *xrrs = XRRSizes(display, 0, &numberSizes);
    
    XRRScreenConfiguration *configuration = XRRGetScreenInfo(display, RootWindow(display, 0));
    SizeID originalSize = XRRConfigCurrentConfiguration(configuration, &originalRotation);
    
    int width = xrrs[originalSize].width;
    int height = xrrs[originalSize].height;
    
    xOrigin = (int)width / 2;
    yOrigin = (int)height / 2;
	
	printf("Server Dimensions: %i x %i.\n", xrrs[originalSize].width, xrrs[originalSize].height);
	printf("Origin of Screen %i: (%i, %i).\n", DefaultScreen(display), xOrigin, yOrigin);
}


void CommandInterpreter::handleCommand(TACommand command)
{
	XTestGrabControl(display, True);

    printf("COMMAND: %i, %i, %i, %i, %f\n", command.type, command.touch, command.xDifference, command.yDifference, command.zoomValue);
	switch(command.type)
	{		
		case TACommandTypeZoom:
			zoom(command);
			break;
		case TACommandTypePan:
			pan(command);
			break;
		case TACommandTypeRotate:
			rotate(command);
			break;
		default:
		break;
	}
	
	XSync(display, 0);
	XTestGrabControl(display, False);
	usleep(10);
}

void CommandInterpreter::moveMouse(TACommand command)
{
	int absX = xOrigin + command.xDifference;
    int absY = yOrigin + command.yDifference;
	
	XTestFakeMotionEvent(display, 0, absX, absY, CurrentTime);
}

void CommandInterpreter::rotate(TACommand command)
{	
	if (currentEvent != NULL)
		cancel(command);
		
	switch (command.touch)
	{
		case TACommandTouchStart:
			currentEvent = TACommandTypeRotate;
			XTestFakeButtonEvent(display, 1, True, CurrentTime);
		break;
		case TACommandTouchMove:
			moveMouse(command);
		break;
		case TACommandTouchEnd:
			XTestFakeButtonEvent(display, 1, False, CurrentTime);
			currentEvent = NULL;
		break;
		default:
		break;
	}
}

void CommandInterpreter::zoom(TACommand command)
{
	int threshold = 12;
	
	if (currentEvent != NULL)
		cancel(command);

	switch (command.touch)
	{
		case TACommandTouchStart:
			currentEvent = TACommandTypeZoom;
			XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
			XTestFakeButtonEvent(display, 3, True, CurrentTime);
		break;
		case TACommandTouchMove:
		{
			if (command.zoomValue > 1.0)
				XTestFakeRelativeMotionEvent(display, 0, threshold, CurrentTime);
			if (command.zoomValue < 1.0)
				XTestFakeRelativeMotionEvent(display, 0, (-1 * threshold), CurrentTime);
		}
		break;
		case TACommandTouchEnd:
		{
			XTestFakeButtonEvent(display, 3, False, CurrentTime);
			//Resets zoom back to it's original position.
			XTestFakeMotionEvent(display, 0, event.xbutton.x, event.xbutton.y, CurrentTime);
			currentEvent = NULL;
		}
		break;
		default:
		break;
	}
}

void CommandInterpreter::pan(TACommand command)
{
	if (currentEvent != NULL)
		cancel(command);
		
	switch (command.touch)
	{
		case TACommandTouchStart:
			currentEvent = TACommandTypePan;
			XTestFakeButtonEvent(display, 2, True, CurrentTime);
		break;
		case TACommandTouchMove:
			moveMouse(command);
		break;
		case TACommandTouchEnd:
			XTestFakeButtonEvent(display, 2, False, CurrentTime);
			currentEvent = NULL;
		break;
		default:
		break;
	}
}

void CommandInterpreter::cancel(TACommand command)
{
	switch (currentEvent)
	{
		case TACommandTypeRotate:
			XTestFakeButtonEvent(display, 1, False, CurrentTime);
		break;
		case TACommandTypeZoom:
			XTestFakeButtonEvent(display, 3, False, CurrentTime);
		break;
		case TACommandTypePan:
			XTestFakeButtonEvent(display, 2, False, CurrentTime);
		break;
		default:
		break;
	}
	currentEvent = NULL;
}