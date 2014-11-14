﻿//
// Extraction.h
//
// $Id: //poco/1.4/Data/include/Poco/Data/Extraction.h#1 $
//
// Library: Data
// Package: DataCore
// Module:  Extraction
//
// Definition of the Extraction class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Data_Extraction_INCLUDED
#define Data_Extraction_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/AbstractExtraction.h"
#include "Poco/Data/Prepare.h"
#include "Poco/Data/TypeHandler.h"
#include "Poco/Data/Column.h"
#include "Poco/Data/DataException.h"
#include <set>
#include <vector>
#include <list>
#include <map>
#include <cstddef>


namespace Poco
{
namespace Data
{


template <class T>
class Extraction: public AbstractExtraction
    /// Concrete Data Type specific extraction of values from a query result set.
{
public:
    Extraction(T& result): _rResult(result), _default(), _extracted(false)
        /// Creates an Extraction object, uses an empty object T as default value
    {
    }

    Extraction(T& result, const T& def): _rResult(result), _default(def), _extracted(false)
        /// Creates an Extraction object, uses the provided def object as default value
    {
    }

    ~Extraction()
    /// Destroys the Extraction object.
    {
    }

    std::size_t numOfColumnsHandled() const
    {
        return TypeHandler<T>::size();
    }

    std::size_t numOfRowsHandled() const
    {
        return 1u;
    }

    std::size_t numOfRowsAllowed() const
    {
        return 1u;
    }

    void extract(std::size_t pos)
    {
        if (_extracted) throw ExtractException("value already extracted");
        _extracted = true;
        TypeHandler<T>::extract(pos, _rResult, _default, getExtractor());
    }

    void reset()
    {
        _extracted = false;
    }

    AbstractPrepare* createPrepareObject(AbstractPreparation* pPrep, std::size_t pos) const
    {
        return new Prepare<T>(pPrep, pos, _default);
    }

private:
    T&   _rResult;
    T    _default;   // copy the default
    bool _extracted;
};


template <class T>
class Extraction<std::vector<T> >: public AbstractExtraction
    /// Vector Data Type specialization for extraction of values from a query result set.
{
public:
    Extraction(std::vector<T>& result): _rResult(result), _default()
    {
    }

    Extraction(std::vector<T>& result, const T& def): _rResult(result), _default(def)
    {
    }

    virtual ~Extraction()
    {
    }

    std::size_t numOfColumnsHandled() const
    {
        return TypeHandler<T>::size();
    }

    std::size_t numOfRowsHandled() const
    {
        return _rResult.size();
    }

    std::size_t numOfRowsAllowed() const
    {
        return getLimit();
    }

    void extract(std::size_t pos)
    {
        _rResult.push_back(_default);
        TypeHandler<T>::extract(pos, _rResult.back(), _default, getExtractor());
    }

    virtual void reset()
    {
    }

    AbstractPrepare* createPrepareObject(AbstractPreparation* pPrep, std::size_t pos) const
    {
        return new Prepare<T>(pPrep, pos, _default);
    }

protected:

    const std::vector<T>& result() const
    {
        return _rResult;
    }

private:
    std::vector<T>& _rResult;
    T               _default; // copy the default
};


template <class T>
class Extraction<std::list<T> >: public AbstractExtraction
    /// List Data Type specialization for extraction of values from a query result set.
{
public:
    Extraction(std::list<T>& result): _rResult(result), _default()
    {
    }

    Extraction(std::list<T>& result, const T& def): _rResult(result), _default(def)
    {
    }

    virtual ~Extraction()
    {
    }

    std::size_t numOfColumnsHandled() const
    {
        return TypeHandler<T>::size();
    }

    std::size_t numOfRowsHandled() const
    {
        return _rResult.size();
    }

    std::size_t numOfRowsAllowed() const
    {
        return getLimit();
    }

    void extract(std::size_t pos)
    {
        _rResult.push_back(_default);
        TypeHandler<T>::extract(pos, _rResult.back(), _default, getExtractor());
    }

    virtual void reset()
    {
    }

    AbstractPrepare* createPrepareObject(AbstractPreparation* pPrep, std::size_t pos) const
    {
        return new Prepare<T>(pPrep, pos, _default);
    }

protected:

    const std::list<T>& result() const
    {
        return _rResult;
    }

private:
    std::list<T>& _rResult;
    T               _default; // copy the default
};


template <class T, class C = std::vector<T> >
class InternalExtraction: public Extraction<C>
    /// Container Data Type specialization extension for extraction of values from a query result set.
    ///
    /// This class is intended for PocoData internal use - it is used by StatementImpl
    /// to automaticaly create internal Extraction in cases when statement returns data and no external storage
    /// was supplied. It is later used by RecordSet to retrieve the fetched data after statement execution.
    /// It takes ownership of the Column pointer supplied as constructor argument. Column object, in turn
    /// owns the data vector pointer.
    ///
    /// InternalExtraction objects can not be copied or assigned.
{
public:
    explicit InternalExtraction(C& result, Column<T>* pColumn):
        Extraction<C>(result),
        _pColumn(pColumn)
        /// Creates InternalExtraction.

    {
    }

    ~InternalExtraction()
    /// Destroys InternalExtraction.
    {
        delete _pColumn;
    }

    void reset()
    {
        _pColumn->reset();
    }

    const T& value(int index) const
    {
        try
        {
            return Extraction<C>::result().at(index);
        }
        catch (std::out_of_range& ex)
        {
            throw RangeException(ex.what());
        }
    }

    const Column<T>& column() const
    {
        return *_pColumn;
    }

private:
    InternalExtraction();
    InternalExtraction(const InternalExtraction&);
    InternalExtraction& operator = (const InternalExtraction&);

    Column<T>* _pColumn;
};


template <class T>
class Extraction<std::set<T> >: public AbstractExtraction
    /// Set Data Type specialization for extraction of values from a query result set.
{
public:
    Extraction(std::set<T>& result): _rResult(result), _default()
    {
    }

    Extraction(std::set<T>& result, const T& def): _rResult(result), _default(def)
    {
    }

    ~Extraction()
    {
    }

    std::size_t numOfColumnsHandled() const
    {
        return TypeHandler<T>::size();
    }

    std::size_t numOfRowsHandled() const
    {
        return _rResult.size();
    }

    std::size_t numOfRowsAllowed() const
    {
        return getLimit();
    }

    void extract(std::size_t pos)
    {
        T tmp;
        TypeHandler<T>::extract(pos, tmp, _default, getExtractor());
        _rResult.insert(tmp);
    }

    void reset()
    {
    }

    AbstractPrepare* createPrepareObject(AbstractPreparation* pPrep, std::size_t pos) const
    {
        return new Prepare<T>(pPrep, pos, _default);
    }

private:
    std::set<T>& _rResult;
    T            _default; // copy the default
};


template <class T>
class Extraction<std::multiset<T> >: public AbstractExtraction
    /// Multiset Data Type specialization for extraction of values from a query result set.
{
public:
    Extraction(std::multiset<T>& result): _rResult(result), _default()
    {
    }

    Extraction(std::multiset<T>& result, const T& def): _rResult(result), _default(def)
    {
    }

    ~Extraction()
    {
    }

    std::size_t numOfColumnsHandled() const
    {
        return TypeHandler<T>::size();
    }

    std::size_t numOfRowsHandled() const
    {
        return _rResult.size();
    }

    std::size_t numOfRowsAllowed() const
    {
        return getLimit();
    }

    void extract(std::size_t pos)
    {
        T tmp;
        TypeHandler<T>::extract(pos, tmp, _default, getExtractor());
        _rResult.insert(tmp);
    }

    void reset()
    {
    }

    AbstractPrepare* createPrepareObject(AbstractPreparation* pPrep, std::size_t pos) const
    {
        return new Prepare<T>(pPrep, pos, _default);
    }

private:
    std::multiset<T>& _rResult;
    T                 _default; // copy the default
};


template <class K, class V>
class Extraction<std::map<K, V> >: public AbstractExtraction
    /// Map Data Type specialization for extraction of values from a query result set.
{
public:
    Extraction(std::map<K, V>& result): _rResult(result), _default()
    {
    }

    Extraction(std::map<K, V>& result, const V& def): _rResult(result), _default(def)
    {
    }

    ~Extraction()
    {
    }

    std::size_t numOfColumnsHandled() const
    {
        return TypeHandler<V>::size();
    }

    std::size_t numOfRowsHandled() const
    {
        return _rResult.size();
    }

    std::size_t numOfRowsAllowed() const
    {
        return getLimit();
    }

    void extract(std::size_t pos)
    {
        V tmp;
        TypeHandler<V>::extract(pos, tmp, _default, getExtractor());
        _rResult.insert(std::make_pair(tmp(), tmp));
    }

    void reset()
    {
    }

    AbstractPrepare* createPrepareObject(AbstractPreparation* pPrep, std::size_t pos) const
    {
        return new Prepare<V>(pPrep, pos, _default);
    }


private:
    std::map<K, V>& _rResult;
    V               _default; // copy the default
};


template <class K, class V>
class Extraction<std::multimap<K, V> >: public AbstractExtraction
    /// Multimap Data Type specialization for extraction of values from a query result set.
{
public:
    Extraction(std::multimap<K, V>& result): _rResult(result), _default()
    {
    }

    Extraction(std::multimap<K, V>& result, const V& def): _rResult(result), _default(def)
    {
    }

    ~Extraction()
    {
    }

    std::size_t numOfColumnsHandled() const
    {
        return TypeHandler<V>::size();
    }

    std::size_t numOfRowsHandled() const
    {
        return _rResult.size();
    }

    std::size_t numOfRowsAllowed() const
    {
        return getLimit();
    }

    void extract(std::size_t pos)
    {
        V tmp;
        TypeHandler<V>::extract(pos, tmp, _default, getExtractor());
        _rResult.insert(std::make_pair(tmp(), tmp));
    }

    void reset()
    {
    }

    AbstractPrepare* createPrepareObject(AbstractPreparation* pPrep, std::size_t pos) const
    {
        return new Prepare<V>(pPrep, pos, _default);
    }

private:
    std::multimap<K, V>& _rResult;
    V                    _default; // copy the default
};


template <typename T> Extraction<T>* into(T& t)
/// Convenience function to allow for a more compact creation of a default extraction object
{
    return new Extraction<T>(t);
}


template <typename T> Extraction<T>* into(T& t, const T& def)
/// Convenience function to allow for a more compact creation of an extraction object with the given default
{
    return new Extraction<T>(t, def);
}


}
} // namespace Poco::Data


#endif // Data_Extraction_INCLUDED
