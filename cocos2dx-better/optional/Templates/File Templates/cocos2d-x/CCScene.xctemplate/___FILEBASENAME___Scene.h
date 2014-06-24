//
//  ___FILENAME___
//  ___PROJECTNAME___
//
//  Created on ___DATE___.
//___COPYRIGHT___
//

#ifndef _____PROJECTNAMEASIDENTIFIER________FILEBASENAMEASIDENTIFIER___Scene__
#define _____PROJECTNAMEASIDENTIFIER________FILEBASENAMEASIDENTIFIER___Scene__

class ___FILEBASENAMEASIDENTIFIER___ : public CCLayer {
protected:
	___FILEBASENAMEASIDENTIFIER___();
	
public:
	virtual ~___FILEBASENAMEASIDENTIFIER___();
    static CCScene* scene();
	
	// override
    virtual bool init();
    virtual void onEnter();
	virtual void onExit();
    
    CREATE_FUNC(___FILEBASENAMEASIDENTIFIER___);
};

#endif /* defined(_____PROJECTNAMEASIDENTIFIER________FILEBASENAMEASIDENTIFIER_____) */
