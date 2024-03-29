//
// Copyright (C) 2011 Xavier MICHEL (xavier.michel.mx440@gmail.com)
//
// This software is provided as-is, without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//

#include "Log.hpp"
#include <Windows.h>


#ifdef SHOW_LOG

Log Log::m_log;

//-------------------------------------------------------------
Log::Log()
    : m_logLevel(eErrLevel_VERBOSE), m_ostream(&std::cout)
{
    // là on choisi le message qui sera affiché pour indiquer le niveau de log
    m_logLabels[eErrLevel_VERBOSE] = "[v] ";
    m_logLabels[eErrLevel_DEBUG]   = "[d] ";
    m_logLabels[eErrLevel_INFO]    = "[i] ";
    m_logLabels[eErrLevel_WARNING] = "[w] ";
    m_logLabels[eErrLevel_ERROR]   = "[e] ";
}

//-------------------------------------------------------------
void Log::writeLine(LogLevel level, const std::string &tag, const std::string &message) {
    if ( level >= m_log.m_logLevel ) {
		std::string dbgMsg = m_log.m_logLabels[level];
		dbgMsg += tag;
		dbgMsg += " : ";
		dbgMsg += message;
		dbgMsg += L'\n';

        *m_log.m_ostream << dbgMsg << std::flush;

#if _WIN32
		OutputDebugString(dbgMsg.c_str());
#endif

    }
    //if( level == eErrLevel_ERROR)
    //	exit(-1);
}

//-------------------------------------------------------------
void Log::setLogLevel(LogLevel l) {
    m_log.m_logLevel = l;
}

//-------------------------------------------------------------
void Log::setOutput(std::ostream &stream) {
    m_log.m_ostream = &stream;
}


#endif // SHOW_LOG
