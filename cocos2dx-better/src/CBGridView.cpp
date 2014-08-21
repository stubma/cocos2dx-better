/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-better
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "CBGridView.h"
#include "CCTableViewCell.h"

USING_NS_CC_EXT;

NS_CC_BEGIN

CBGridView::CBGridView() :
m_colCount(1) {
}

CBGridView* CBGridView::create(CBTableViewDataSource* dataSource, CCSize size) {
    return CBGridView::create(dataSource, size, NULL);
}

CBGridView* CBGridView::create(CBTableViewDataSource* dataSource, CCSize size, CCNode* container) {
    CBGridView* table = new CBGridView();
    if(table->initWithViewSize(size, container)) {
        table->autorelease();
        table->setDataSource(dataSource);
        table->_updateContentSize();
        return table;
    }
    CC_SAFE_RELEASE(table);
    return NULL;
}

void CBGridView::setColCount(unsigned int cols) {
    m_colCount = cols;
    _updateContentSize();
}

int CBGridView::__indexFromOffset(CCPoint offset) {
    int index = 0;
    CCSize cellSize;
    int col, row;
    float spaceWidth;
    
    cellSize = m_pDataSource->cellSizeForTable(this);
    switch(getDirection()) {
        case kCCScrollViewDirectionHorizontal:
            spaceWidth = getContainer()->getContentSize().height / m_colCount;
            col = (offset.y - (spaceWidth - cellSize.height) * 0.5) / spaceWidth;
            row = offset.x / cellSize.width;
            break;
        default:
            spaceWidth = getContainer()->getContentSize().width / m_colCount;
            col = (offset.x - (spaceWidth - cellSize.width) * 0.5) / spaceWidth;
            row = MAX(0, offset.y / cellSize.height);

            break;
    }
    index = col + row * m_colCount;
    return index;
}

CCPoint CBGridView::__offsetFromIndex(unsigned int index) {
    CCPoint offset;
    CCSize  cellSize;
    float spaceWidth;
    int col, row;
    
    cellSize = m_pDataSource->cellSizeForTable(this);
    switch(getDirection()) {
        case kCCScrollViewDirectionHorizontal:
            row = index / m_colCount;
            col = index % m_colCount;
            spaceWidth = this->getContainer()->getContentSize().height / m_colCount;
            offset = ccp(row * cellSize.height,
                         col * spaceWidth + (spaceWidth - cellSize.width) * 0.5);
            break;
        default:
            row = index / m_colCount;
            col = index % m_colCount;
            spaceWidth = this->getContainer()->getContentSize().width / m_colCount;
            offset = ccp(col * spaceWidth + (spaceWidth - cellSize.width) * 0.5,
                         row * cellSize.height);

            break;
    }
    
    return offset;
    
}

void CBGridView::_updateCellPositions() {
    int cellsCount = m_pDataSource->numberOfCellsInTableView(this);
    m_vCellsPositions.resize(cellsCount + 1, 0.0);
    
    if(cellsCount > 0) {
        float currentPos = 0;
        CCSize cellSize;
        for(int i = 0; i < cellsCount; i++) {
            if(i > 0 && i % m_colCount == 0) {
                cellSize = m_pDataSource->tableCellSizeForIndex(this, i);
                switch(getDirection()) {
                    case kCCScrollViewDirectionHorizontal:
                        currentPos += cellSize.width;
                        break;
                    default:
                        currentPos += cellSize.height;
                        break;
                }
            }
            m_vCellsPositions[i] = currentPos;
        }
        
        // 1 extra value allows us to get right/bottom of the last cell
        m_vCellsPositions[cellsCount] = currentPos;
    }
}

void CBGridView::_updateContentSize() {
    CCSize size, cellSize, viewSize;
    unsigned int cellCount, rows;
    
    cellSize = m_pDataSource->cellSizeForTable(this);
    cellCount = m_pDataSource->numberOfCellsInTableView(this);
    viewSize = CCSizeMake(getViewSize().width/getContainer()->getScaleX(),
                          getViewSize().height / getContainer()->getScaleY());
    
    switch(getDirection()) {
        case kCCScrollViewDirectionHorizontal:
            rows = ceilf(cellCount / (float)m_colCount);
            size = CCSizeMake(MAX(rows * cellSize.width, viewSize.width),
                              m_colCount * cellSize.height);
            break;
        default:
            rows = ceilf(cellCount/((float)m_colCount));
            size = CCSizeMake(MAX(cellSize.width * m_colCount, viewSize.width),
                              MAX(rows * cellSize.height, viewSize.height));
            break;
    }
    setContentSize(size);
}

NS_CC_END
