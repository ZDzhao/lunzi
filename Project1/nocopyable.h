
#ifndef nocopyable_h__
#define nocopyable_h__

class noncopyable
{
protected:
	noncopyable(void){}
	~noncopyable(void){}

private:  // emphasize the following members are private  
	noncopyable(const noncopyable&);
	const noncopyable& operator= (const noncopyable&);
};

#endif // nocopyable_h__