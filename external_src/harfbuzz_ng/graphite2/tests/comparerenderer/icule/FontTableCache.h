/*
 **********************************************************************
 *   Copyright (C) 2003-2008, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 **********************************************************************
 */

#ifndef __FONTTABLECACHE_H

#define __FONTTABLECACHE_H

#include "layout/LETypes.h"

U_NAMESPACE_USE

struct IcuFontTableCacheEntry;

class IcuFontTableCache
{
public:
    IcuFontTableCache();

    virtual ~IcuFontTableCache();

    const void *find(LETag tableTag) const;

protected:
    virtual const void *readFontTable(LETag tableTag) const = 0;
    virtual void freeFontTable(const void *table) const;

private:

    void add(LETag tableTag, const void *table);

    IcuFontTableCacheEntry *fTableCache;
    le_int32 fTableCacheCurr;
    le_int32 fTableCacheSize;
};

#endif

