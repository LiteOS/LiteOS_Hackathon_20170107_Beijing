//
//  HAMClientVC.m
//  HAMDriverlessCarController
//
//  Created by DaiYue’s Macbook on 2017/1/7.
//  Copyright © 2017年 Find the Lamp Studio. All rights reserved.
//

#import "HAMClientVC.h"

#import "UIColor+WNYUtils.h"

#import "SVProgressHUD.h"

#import "ControlSocketManager.h"
#import "ConfigData.h"
#import "TransData.h"

@interface HAMTileInfo : NSObject

@property (nonatomic, assign) NSInteger x;
@property (nonatomic, assign) NSInteger y;

@property (nonatomic, assign) BOOL canGoUp;
@property (nonatomic, assign) BOOL canGoDown;
@property (nonatomic, assign) BOOL canGoLeft;
@property (nonatomic, assign) BOOL canGoRight;

+ (instancetype)tileThatCanGoUp:(BOOL)canGoUp down:(BOOL)canGoDown left:(BOOL)canGoLeft right:(BOOL)canGoRight;

@end

@implementation HAMTileInfo

+ (instancetype)tileThatCanGoUp:(BOOL)canGoUp down:(BOOL)canGoDown left:(BOOL)canGoLeft right:(BOOL)canGoRight {
    HAMTileInfo* tileInfo = [HAMTileInfo new];

    tileInfo.canGoUp = canGoUp;
    tileInfo.canGoDown = canGoDown;
    tileInfo.canGoLeft = canGoLeft;
    tileInfo.canGoRight = canGoRight;
    
    return tileInfo;
}

@end

@interface HAMClientVC ()
@property (weak, nonatomic) IBOutlet UIButton *confirmButton;

@property (weak, nonatomic) IBOutlet UIImageView *carImageView;

@property (strong, nonatomic) IBOutletCollection(UIView) NSArray *tiles;
@property (nonatomic, strong) UIView* targetTile;

// map
@property (nonatomic, strong) NSArray* tilesMatrix;
@property (nonatomic, strong) NSArray* tilesInfos;

@property (nonatomic, assign) BOOL carIsMoving;

@end

@implementation HAMClientVC

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // view
    self.confirmButton.layer.borderWidth = 1.0f;
    self.confirmButton.layer.borderColor = [[UIColor colorWithHex:0x87b345] CGColor];
    
    // data
    [self prepareData];
    
    [self carStartScaleAnimation];
    
    // network
}

- (void)viewWillAppear:(BOOL)animated {
    self.carIsMoving = NO;
}

- (void)prepareData {
    NSMutableArray* matrix = [NSMutableArray array];
    for (NSInteger i = 0; i < 5; i++) {
        NSMutableArray* row = [NSMutableArray array];
        for (NSInteger j = 0; j < 5; j++) {
            NSInteger index = i * 5 + j;
            UIView* tile = self.tiles[index];
            [row addObject:tile];
            tile.tag = index;
            [tile addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tileTapedAction:)]];
        }
        [matrix addObject:[row copy]];
    }
    self.tilesMatrix = [matrix copy];
    
    // map
    NSMutableArray* map = [NSMutableArray array];
        // row1
    [map addObject:[HAMTileInfo tileThatCanGoUp:NO down:YES left:NO right:YES]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:NO down:NO left:YES right:YES]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:NO down:YES left:YES right:NO]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:NO down:NO left:NO right:YES]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:NO down:YES left:YES right:NO]];
        // row 2
    [map addObject:[HAMTileInfo tileThatCanGoUp:YES down:YES left:NO right:NO]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:NO down:YES left:NO right:YES]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:YES down:NO left:YES right:NO]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:NO down:YES left:NO right:YES]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:YES down:YES left:YES right:NO]];
        // row 3
    [map addObject:[HAMTileInfo tileThatCanGoUp:YES down:NO left:NO right:NO]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:YES down:YES left:NO right:YES]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:NO down:NO left:YES right:YES]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:YES down:NO left:YES right:NO]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:YES down:YES left:NO right:NO]];
        // row 4
    [map addObject:[HAMTileInfo tileThatCanGoUp:NO down:YES left:NO right:YES]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:YES down:NO left:YES right:NO]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:NO down:YES left:NO right:YES]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:NO down:NO left:YES right:NO]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:YES down:YES left:NO right:NO]];
        // row 5
    [map addObject:[HAMTileInfo tileThatCanGoUp:YES down:NO left:NO right:YES]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:NO down:NO left:YES right:YES]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:YES down:NO left:YES right:YES]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:NO down:NO left:YES right:YES]];
    [map addObject:[HAMTileInfo tileThatCanGoUp:YES down:NO left:YES right:NO]];
    self.tilesInfos = [map copy];
}

#pragma mark - Actions

- (IBAction)backAction:(id)sender {
    [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)comfirmAction:(id)sender {
    NSInteger tag = self.targetTile.tag;
    [self moveCarTox:tag / 5 y:tag % 5];
}

#pragma mark - Car

- (void)tileTapedAction:(UIGestureRecognizer*)recognizer {
    if (self.carIsMoving) {
        return;
    }
    
    NSInteger tag = recognizer.view.tag;
    UIView* tile = self.tiles[tag];
    tile.backgroundColor = [UIColor colorWithHex:0xc2d99e];
    
    self.targetTile = tile;
}

- (void)moveCarTox:(NSInteger)x y:(NSInteger)y {
    self.carIsMoving = YES;
    [self carStartScaleAnimation];
    
    [SVProgressHUD showWithStatus:@"正在寻路..."];
    NSArray* path = [self findPathFromX:1 fromY:0 toX:x toY:y];
    if (!path) {
        [SVProgressHUD showErrorWithStatus:@"抱歉，寻路失败"];
        return;
    }
    [SVProgressHUD showSuccessWithStatus:@"寻路成功！"];
    for (NSInteger i = 1; i < path.count - 1; i++) { // avoid start & end
        UIView* tile = path[i];
        tile.backgroundColor = [UIColor colorWithHex:0xdae8c4];
    }
    [self animateCarThroughPath:path];
}

- (NSArray*)findPathFromX:(NSInteger)fromX fromY:(NSInteger)fromY toX:(NSInteger)toX toY:(NSInteger)toY {
    NSArray* paths = [NSMutableArray arrayWithObject:@[self.tilesMatrix[fromX][fromY]]];
    while (paths.count > 0) {
        NSMutableArray* newPaths = [NSMutableArray array];
        for (NSArray* path in paths) {
            UIView* lastTile = [path lastObject];
            if (lastTile.tag == toX * 5 + toY) {
                // found path
                return path;
            }
            
            NSInteger lastX = lastTile.tag / 5;
            NSInteger lastY = lastTile.tag % 5;
            HAMTileInfo* lastTileInfo = self.tilesInfos[lastTile.tag];
            if (lastTileInfo.canGoUp) {
                NSMutableArray* newPath = [path mutableCopy];
                UIView* upTile = self.tilesMatrix[lastX - 1][lastY];
                if (![path containsObject:upTile]) {
                    [newPath addObject:upTile];
                    [newPaths addObject:[newPath copy]];
                }
            }
            if (lastTileInfo.canGoDown) {
                NSMutableArray* newPath = [path mutableCopy];
                UIView* downTile = self.tilesMatrix[lastX + 1][lastY];
                if (![path containsObject:downTile]) {
                    [newPath addObject:downTile];
                    [newPaths addObject:[newPath copy]];
                }
            }
            if (lastTileInfo.canGoLeft) {
                NSMutableArray* newPath = [path mutableCopy];
                UIView* leftTile = self.tilesMatrix[lastX][lastY - 1];
                if (![path containsObject:leftTile]) {
                    [newPath addObject:leftTile];
                    [newPaths addObject:[newPath copy]];
                }
            }
            if (lastTileInfo.canGoRight) {
                NSMutableArray* newPath = [path mutableCopy];
                UIView* rightTile = self.tilesMatrix[lastX][lastY + 1];
                if (![path containsObject:rightTile]) {
                    [newPath addObject:rightTile];
                    [newPaths addObject:[newPath copy]];
                }
            }
        }
        paths = newPaths;
    }
    return nil;
}

#pragma mark - Animation

- (void)carStartScaleAnimation {
    [UIView animateWithDuration:0.8f delay:0.0f options:UIViewAnimationOptionRepeat| UIViewAnimationOptionAutoreverse animations:^{
        self.carImageView.transform = CGAffineTransformMakeScale(1.1, 1.1);
    } completion:^(BOOL finished) {
        
    }];
}

- (void)animateCarThroughPath:(NSArray*)path {
    [self animateCartThroughPath:path nextTileIndex:1];
}

- (void)animateCartThroughPath:(NSArray*)path nextTileIndex:(NSInteger)i {
    [UIView animateWithDuration:3.5f delay:0.0f options:UIViewAnimationOptionCurveLinear animations:^{
        UIView* tile = path[i];
        self.carImageView.center = tile.center;
    } completion:^(BOOL finished) {
        if (i == path.count - 1) {
            [SVProgressHUD showSuccessWithStatus:@"碰碰博士已到达，请取货"];
            [self performSegueWithIdentifier:@"deliverySegue" sender:self];
        } else {
            [self animateCartThroughPath:path nextTileIndex:i + 1];
        }
    }];
}

@end
