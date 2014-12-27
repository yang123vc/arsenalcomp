//
//  AppDelegate.m
//  Test
//
//  Created by peng liu on 2/20/12.
//  Copyright (c) 2012 none. All rights reserved.
//

#import "ArsenalWrapper.h"
#import "AppDelegate.h"



#include <mach/mach.h>

#include <mach/mach_time.h>

#include <pthread.h>





@implementation AppDelegate

@synthesize window = _window;

- (void)dealloc
{
        [_window release];
    [super dealloc];
}

static void	thread_worker(void *data)
{
        ar_int_64_t beg, end;
        
        while(true)
        {
                beg = AR_GetTime_TickCount();

                AR_Sleep(3);
                
                end = AR_GetTime_TickCount();
                
                AR_printf(L"elapsed : %qd\r\n", end- beg);
        }
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
        Initialize_Arsenal();
        
        arThread_t *thd = AR_CreateThread(thread_worker, NULL);
        if(thd)
        {
                AR_SetThreadPriority(thd, AR_THREAD_PREC_HIGH);
                
        }
        
        
        
        return YES;
}
							
- (void)applicationWillResignActive:(UIApplication *)application
{
        /*
         Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
         Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
         */
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
        /*
         Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
         If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
         */
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
        /*
         Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
         */
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
        /*
         Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
         */
}

- (void)applicationWillTerminate:(UIApplication *)application
{
        /*
         Called when the application is about to terminate.
         Save data if appropriate.
         See also applicationDidEnterBackground:.
         */
        
       UnInitialize_Arsenal();
        
}

@end
