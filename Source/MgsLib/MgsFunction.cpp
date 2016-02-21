#include "stdafx.h"
#include "MgsFunction.hpp"
#include <set>

struct TVarInfo
{
    DWORD mAddr;
    DWORD mSize;
    bool mIsPointerType;
    bool mIsConstData;
    const char* mName;
};

// Since the MgsFunctionBase instances are global we have to do this to avoid global static init ordering issues
/*static*/ std::map<DWORD, MgsFunctionBase*>& MgsFunctionBase::GetMgsFunctionTable()
{
    static std::map<DWORD, MgsFunctionBase*> table;
    return table;
}

bool operator < (const TVarInfo& lhs, const TVarInfo& rhs)
{
    return lhs.mAddr < rhs.mAddr;
}

std::set<TVarInfo>& Vars()
{
    static std::set<TVarInfo> v;
    return v;
}

MgsVar::MgsVar(const char* name, DWORD addr, DWORD sizeInBytes, bool isPointerType, bool isConstData)
{
    for (const auto& var : Vars())
    {
        if (var.mAddr == addr)
        {
            // Var has been defined twice
            LOG_ERROR("Var at addr " << var.mAddr << " is defined more than once");
            abort();
        }
        else if (var.mAddr >= addr && var.mAddr + var.mSize <= addr)
        {
            // Var overlaps
            LOG_ERROR("Var at addr " << var.mAddr << " overlaps another var");
            abort();
        }
    }
    Vars().insert({ addr, sizeInBytes, isPointerType, isConstData, name });
}

/*static*/ void  MgsVar::TrackAlloc(void* ptr, size_t size)
{
    AutoCsLock lock(mCs);
    mAllocs[ptr] = size;
}

/*static*/ void  MgsVar::TrackFree(void* ptr)
{
    AutoCsLock lock(mCs);
    auto it = mAllocs.find(ptr);
    if (it != std::end(mAllocs))
    {
        mAllocs.erase(it);
    }
}

/*static*/ AutoCs MgsVar::mCs;
/*static*/ std::map<void*, size_t> MgsVar::mAllocs;

/*static*/ std::unique_ptr<MgsVar::SnapShot> MgsVar::MakeSnapShot()
{
    AutoCsLock lock(mCs);
    return std::make_unique<MgsVar::SnapShot>();
}

MgsVar::SnapShot::SnapShot()
{
    // Take a copy of the vars at this point in time
    for (const TVarInfo& v : Vars())
    {
        // Const data is read only, trying to write to it will attempt to write back the same value
        // and crash trying to do. This could be improved to snapshot only and then have the restore
        // assert it didn't change.
        if (!v.mIsConstData)
        {
            if (v.mIsPointerType)
            {
                mVars[v.mAddr] = std::make_unique<DynamicVarSnapShot>(v.mName, v.mAddr);
            }
            else
            {
                mVars[v.mAddr] = std::make_unique<StaticVarSnapShot>(v.mName, v.mAddr, v.mSize);
            }
        }
    }
}

void MgsVar::SnapShot::AssertEqual(const SnapShot& other)
{
    for (const auto& varSnap : mVars)
    {
        const auto& otherSnap = other.mVars.find(varSnap.second->Address());

        auto tmpLhs = dynamic_cast<StaticVarSnapShot*>(varSnap.second.get());
        if (tmpLhs)
        {
            // Compare static vars
            auto lhs = dynamic_cast<StaticVarSnapShot*>(varSnap.second.get());
            auto rhs = dynamic_cast<StaticVarSnapShot*>(otherSnap->second.get());
            if (*lhs != *rhs)
            {
                abort();
            }
        }
        else
        {
            // Compare dynamic vars
            auto lhs = dynamic_cast<DynamicVarSnapShot*>(varSnap.second.get());
            auto rhs = dynamic_cast<DynamicVarSnapShot*>(otherSnap->second.get());
            if (*lhs != *rhs)
            {
                abort();
            }
        }
    }
}

void MgsVar::SnapShot::Restore()
{
    for (auto& varSnap : mVars)
    {
        varSnap.second->Restore();
    }
}

void StaticVarSnapShot::Restore()
{
    BYTE* value = ((BYTE*)mAddr);
    memcpy(value, mValue.data(), mSize);
}

void StaticVarSnapShot::SnapShot()
{
    BYTE* value = ((BYTE*)mAddr);

    mValue.resize(mSize);
    memcpy(mValue.data(), value, mSize);
}

bool StaticVarSnapShot::operator == (const StaticVarSnapShot& other)
{
    if (mAddr != other.mAddr)
    {
        abort();
    }

    if (mSize != other.mSize)
    {
        abort();
    }

    if (other.mValue != mValue)
    {
        

        LOG_WARNING("Var diff: " << other.mAddr << " name " << other.mName);
        //abort();
    }

    return true;
}

void DynamicVarSnapShot::Restore()
{
    void* pointerValue = (*(DWORD**)mAddr);

    if (DWORD(pointerValue) != mPtrValue)
    {
        LOG_WARNING("Pointer value changed, can't restore as original might have been freed!");
    }
    else
    {
        void* pointerValue = (*(DWORD**)mAddr);
        memcpy(pointerValue, mBufferCopy.data(), mBufferCopy.size());
    }
}

void DynamicVarSnapShot::SnapShot()
{
    // Get the address
    void* pointerValue = (*(DWORD**)mAddr);

    // See if its in the alloc map
    auto it = MgsVar::mAllocs.find(pointerValue);

    if (it != std::end(MgsVar::mAllocs))
    {
        // It is, copy the buffer that we point to
        const size_t allocSize = it->second;
        mBufferCopy.resize(allocSize);
        memcpy(mBufferCopy.data(), pointerValue, allocSize);
    }

    // And always copy the pointer value itself
    mPtrValue = (DWORD)pointerValue;
}

bool DynamicVarSnapShot::operator == (const DynamicVarSnapShot& other)
{
    if (mAddr != other.mAddr)
    {
        abort();
    }

    if (mPtrValue != other.mPtrValue)
    {
        abort();
    }

    if (mBufferCopy != other.mBufferCopy)
    {
        LOG_WARNING("Var diff: " << other.mAddr << " name " << other.mName);
        //abort();
    }

    return true;
}
