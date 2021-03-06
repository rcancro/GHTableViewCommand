//
//  PITableViewCommand.h
//
//  Created by ricky cancro on 2/6/14.
//

@import UIKit;
#import "PIAutoUpdateProtocols.h"

typedef NS_ENUM (NSUInteger, PICommandType)
{
    kPICommandUpdateRow,
    kPICommandAddRow,
    kPICommandRemoveRow,
    
    kPICommandRemoveSection,
    kPICommandAddSection,
};


/**
 *  Category to return an array of index paths from largest section, largest row to smallest section smallest row.
 *  This will allow for a table's data model to delete without worrying about indexes shifting by removing
 *  a low number section before a high number.
 *
 *  This method will properly sort if all the objects are either NSIndexPaths or PITableViewCommand objects.
 */
@interface NSArray(NSIndexPath)
- (NSArray *)deleteFriendlySortedArray;
@end

@class PIAutoUpdateCommand;
/**
 *  Call back when a PITableViewCommand is about to execute (since the commands are not actually issued one at a time
 *  but instead are combined and excuted at the same time, it is difficult to call the callback JUST before -- or after --
 *  the table update command.  However, since the callback will happen between a UITableView's beginUpadte and endUpdate
 *  the timing is not that important.)
 *
 *  @param command The PITableViewCommand that will be excuted.  At this point resolvedIndexPath should be non-nil
 */
typedef void (^PIAutoUpdateCommandCallbackBlock)(PIAutoUpdateCommand *command);

/**
 *  A table command encapsulated in a class.  These commands are run by the UITableView update category method, but are sent along via the callback block
 *  so the client can see which command it is getting a call back about.
 */
@interface PIAutoUpdateCommand : NSObject

/**
 *  The type of command that will be exectued.  See the PITableCommandType enum above.
 */
@property (nonatomic, assign, readonly) PICommandType commandType;

/**
 *  The row that will be affected (if applicable)
 */
@property (nonatomic, assign, readonly) NSUInteger row;

/**
 *  The section Identifier that will be affected (if applicable)
 */
@property (nonatomic, copy, readonly) NSString *sectionIdentifier;

/**
 *  nil until the index path is resolved right before the actual table update is performed.
 *  this property will be valid when TableCommandCallbackBlock is called.
 */
@property (nonatomic, strong, readonly) NSIndexPath *resolvedIndexPath;

@end

typedef void (^PIAutoUpdateRunCommandsBlock) ();
@protocol PIAutoUpdateableCollectionView <NSObject>

- (void)removeSections:(NSIndexSet *)sections;
- (void)insertSections:(NSIndexSet *)sections;

- (void)removeItems:(NSArray *)indexPaths;
- (void)insertItems:(NSArray *)indexPaths;
- (void)refreshItems:(NSArray *)indexPaths;

- (void)runAutoUpdateCommandBlock:(PIAutoUpdateRunCommandsBlock)block;

@end

typedef void (^PIUpdateCollectionViewDataBlock)();

@interface PIAutoUpdateCommandManager : NSObject

+ (void)runCommandsWithAutoUpdateDataSource:(id<PIAutoUpdateDataSource>)updateDataSource
                       autoUpdateableObject:(id<PIAutoUpdateableCollectionView>)updateableObject
                                updateBlock:(PIUpdateCollectionViewDataBlock)updateBlock
                       commandCallbackblock:(PIAutoUpdateCommandCallbackBlock)commandCallbackBlock;

@end

