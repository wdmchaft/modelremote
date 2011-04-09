//
//  TouchpadView.m
//  Troy
//
//  Created by Max Goedjen on 4/8/11.
//  UHD
//

#import "TouchpadView.h"


@implementation TouchpadView

@synthesize delegate;

- (id)init
{
    self = [super init];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (BOOL)canBecomeFirstResponder {
	return YES;
}

- (void)dealloc
{
    [super dealloc];
}

- (void)configure {

    self.image = [UIImage imageNamed:@"iPadControlPadBackground.png"];
    self.userInteractionEnabled = YES;
    self.multipleTouchEnabled = YES;

}

#pragma mark touch interpretation
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    
	CGPoint point = [[touches anyObject] locationInView:nil];
    [delegate touchpadDidTouchPoint:point];
    
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	
    NSLog(@"%i", [touches count]);
	CGPoint point = [[touches anyObject] locationInView:nil];
    [delegate touchpadDidTouchPoint:point];
	
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	
	
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
		
}

@end
