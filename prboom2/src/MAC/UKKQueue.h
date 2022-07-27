/* =============================================================================
	FILE:		UKKQueue.h
	PROJECT:	Filie

    COPYRIGHT:  (c) 2003 M. Uli Kusterer, all rights reserved.

	AUTHORS:	M. Uli Kusterer - UK

    LICENSES:   GPL, Modified BSD

	REVISIONS:
		2003-12-21	UK	Created.
   ========================================================================== */

// -----------------------------------------------------------------------------
//  Headers:
// -----------------------------------------------------------------------------

#import <Foundation/Foundation.h>
#include <sys/types.h>
#include <sys/event.h>
#import "UKFileWatcher.h"


// -----------------------------------------------------------------------------
//  Constants:
// -----------------------------------------------------------------------------

#ifndef UKKQUEUE_BACKWARDS_COMPATIBLE
#define UKKQUEUE_BACKWARDS_COMPATIBLE 1     // 1 to send old-style kqueue:receivedNotification:forFile: messages to objects that accept them.
#endif

// Flags for notifyingAbout:
#define UKKQueueNotifyAboutRename					NOTE_RENAME		// Item was renamed.
#define UKKQueueNotifyAboutWrite					NOTE_WRITE		// Item contents changed (also folder contents changed).
#define UKKQueueNotifyAboutDelete					NOTE_DELETE		// item was removed.
#define UKKQueueNotifyAboutAttributeChange			NOTE_ATTRIB		// Item attributes changed.
#define UKKQueueNotifyAboutSizeIncrease				NOTE_EXTEND		// Item size increased.
#define UKKQueueNotifyAboutLinkCountChanged			NOTE_LINK		// Item's link count changed.
#define UKKQueueNotifyAboutAccessRevocation			NOTE_REVOKE		// Access to item was revoked.

// Notifications this sends:
//  (see UKFileWatcher)
// Old names: *deprecated*
#define UKKQueueFileRenamedNotification				UKFileWatcherRenameNotification
#define UKKQueueFileWrittenToNotification			UKFileWatcherWriteNotification
#define UKKQueueFileDeletedNotification				UKFileWatcherDeleteNotification
#define UKKQueueFileAttributesChangedNotification   UKFileWatcherAttributeChangeNotification
#define UKKQueueFileSizeIncreasedNotification		UKFileWatcherSizeIncreaseNotification
#define UKKQueueFileLinkCountChangedNotification	UKFileWatcherLinkCountChangeNotification
#define UKKQueueFileAccessRevocationNotification	UKFileWatcherAccessRevocationNotification


// -----------------------------------------------------------------------------
//  UKKQueue:
// -----------------------------------------------------------------------------

@interface UKKQueue : NSObject <UKFileWatcher>
{
    int				queueFD;			// The actual queue ID.
    NSMutableArray* watchedPaths;		// List of NSStrings containing the paths we're watching.
    NSMutableArray* watchedFDs;			// List of NSNumbers containing the file descriptors we're watching.
    id				delegate;			// Gets messages about changes instead of notification center, if specified.
    id				delegateProxy;		// Proxy object to which we send messages so they reach delegate on the main thread.
    BOOL			alwaysNotify;		// Send notifications even if we have a delegate? Defaults to NO.
    BOOL			keepThreadRunning;	// Termination criterion of our thread.
}

+(UKKQueue*)    sharedQueue;        // Returns a singleton, a shared kqueue object Handy if you're subscribing to the notifications. Use this, or just create separate objects using alloc/init. Whatever floats your boat.

-(int)  queueFD;		// I know you unix geeks want this...

// High-level file watching: (use UKFileWatcher protocol methods instead, where possible!)
-(void) addPathToQueue: (NSString*)path;
-(void) addPathToQueue: (NSString*)path notifyingAbout: (u_int)fflags;
-(void) removePathFromQueue: (NSString*)path;

-(id)	delegate;
-(void)	setDelegate: (id)newDelegate;

-(BOOL)	alwaysNotify;
-(void)	setAlwaysNotify: (BOOL)n;

// private:
-(void)		watcherThread: (id)sender;
-(void)		postNotification: (NSString*)nm forFile: (NSString*)fp; // Message-posting bottleneck.

@end


// -----------------------------------------------------------------------------
//  Methods delegates need to provide:
//      * DEPRECATED * use UKFileWatcher delegate methods instead!
// -----------------------------------------------------------------------------

@interface NSObject(UKKQueueDelegate)

-(void) kqueue: (UKKQueue*)kq receivedNotification: (NSString*)nm forFile: (NSString*)fpath;

@end
