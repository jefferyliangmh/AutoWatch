#pragma once

#ifndef LOGGER_EXPORTS
#define LOGGER_API __declspec(dllimport)
#else
#define LOGGER_API __declspec(dllexport)
#endif

#define BOOST_ALL_DYN_LINK 
#define BOOST_LIB_DIAGNOSTIC
#define BOOST_AUTO_LINK_TAGGED
