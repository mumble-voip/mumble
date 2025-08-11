#import <Cocoa/Cocoa.h>

static NSString *currentAppAppearance = nil;

void setDarkAppearance()
{
    NSAppearance *darkAppearance = [NSAppearance appearanceNamed:NSAppearanceNameDarkAqua];
    [NSApp setAppearance:darkAppearance];
}

void setLightAppearance()
{
    NSAppearance *lightAppearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    [NSApp setAppearance:lightAppearance];
}

// Revert to system behavior
// When resseting the appearance, we need to check if the system isn't already on the nil appearance
// If not checked, mumble will fall into a loop of trying to apply the theme, calling resetAppearance,
// then sending an event to change the theme. Creating a loop.
void resetAppearanceToSystem() {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (NSApp.appearance != nil) {
            currentAppAppearance = nil;
            [NSApp setAppearance:nil];
        }
    });
}
