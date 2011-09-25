/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import "Common.h"#import "ARException.h"/********************************同步**********************************//********************************Event**********************************//*此对象实现了Win32下的Event对象的功能*/@interface AREvent : NSObject {		NSCondition		*cond;		BOOL			isAutoReset;				BOOL			state;}-(id)	initEvent : (BOOL)is_auto;-(void) set;-(void) reset;-(void)	wait;-(BOOL)	waitTimeout : (NSTimeInterval) milliseconds;@end/********************************读写锁**********************************/@interface ARRWLock : NSObject {		pthread_rwlock_t		rwLock;		}-(id)	init;-(void) dealloc;-(void) readLock;-(BOOL) tryReadLock;-(void)	writeLock;-(BOOL)	tryWriteLock;-(void)	unLock;@end/**********************************************************************//********************************线程控制*******************************/@class ThreadData;@interface ARThread : NSThread {		pthread_t		thread;		AREvent			*event;				ThreadData		*data;}+(void)	sleep : (NSTimeInterval)milliseconds;-(id)	init;-(id)	initWithTarget : (id)obj 			selector : (SEL)sel 			argument : (id)arg						;-(void)			join;-(BOOL)			joinTimeout : (NSTimeInterval) milliseconds;-(BOOL)			isActive;-(void)			getPriority : (int*)prio;-(void)			setPriority : (int)prio;@end/********************************同步辅助工具*******************************//********************************异步队列*******************************/@interface ARAsyncQueue : NSObject{		NSLock				*lock;		NSMutableArray		*waitQueue;		NSMutableArray		*dataQueue;}-(id)	init;-(void)	dealloc;-(BOOL)	hasIdleThread;-(BOOL)	isEmpty;-(void)	clear;-(id)	getData;-(id)	getDataTimeout : (NSTimeInterval) milliseconds;-(void)	putData : (id)data;@end/********************************双向通信的任务*******************************/#define AR_DUPLEX_TASK_MSGID_TO_MAIN			100#define AR_DUPLEX_TASK_MSGID_TO_WORKER			101#define AR_DUPLEX_TASK_MSGID_TO_WORKER_EXIT		102@class ARDuplexTaskWorker;@class ARDuplexTask;@interface ARDuplexTaskMessage : NSObject{		NSString		*name;}@property (retain) NSString* name;-(id)init;-(id)initWithName : (NSString*)n;@end@protocol ARDuplexTaskDelegate-(void)onWorkerBegin;-(void)onWorkerEnd;-(void)handleWorkerThreadMessage : (ARDuplexTaskMessage*)msg forTask : (ARDuplexTask*)task;-(void)handleMainThreadMessage : (ARDuplexTaskMessage*)msg forTask : (ARDuplexTask*)task;@end@interface ARDuplexTask : NSObject<NSMachPortDelegate>{		BOOL									isStarted;		NSObject<ARDuplexTaskDelegate>			*delegate;				NSMachPort								*mainPort;		NSMachPort								*workerPort;				ARAsyncQueue							*mainQueue;		ARAsyncQueue							*workerQueue;				ARDuplexTaskWorker						*worker;		ARThread								*workerThread;}-(id)init;-(void)dealloc;-(NSMachPort*)getMainPort;-(NSMachPort*)getWorkerPort;-(void)setDelegate : (NSObject<ARDuplexTaskDelegate>*)newDelegate;-(NSObject<ARDuplexTaskDelegate>*) delegate;-(void)PostMsgToWorker	:		(ARDuplexTaskMessage*)msg;-(void)PostMsgToMain	:		(ARDuplexTaskMessage*)msg;-(void)start;-(void)stop;@end/**********************************************************************/