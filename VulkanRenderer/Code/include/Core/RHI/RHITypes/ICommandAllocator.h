#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class ICommandAllocator
	{
	public:		
		
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const RHI_RESULT CreateCommandAllocator() = 0;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const RHI_RESULT  DestroyCommandAllocator() = 0;
	};
}