#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "Singleton.h"

namespace Common{

	//-------------------------------------------------------------
	class Manager: public Singleton<Manager>
	{
		friend class Singleton<Manager>;

	public:
		Manager(){}
		virtual ~Manager(void){};

		virtual void	update	(){};
		virtual void	init	(){};
		virtual void	kill	(){};
	};

}

#endif