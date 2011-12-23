//
// Copyright (C) 2004 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#ifndef __REASE_REASEDEFS_H
#define __REASE_REASEDEFS_H

#if OMNETPP_VERSION < 0x0302
#  error At least OMNeT++/OMNEST version 4.0 required
#endif

#if defined(REASE_EXPORT)
#  define REASE_API OPP_DLLEXPORT
#elif defined(REASE_IMPORT)
#  define REASE_API OPP_DLLIMPORT
#else
#  define REASE_API
#endif


#endif
