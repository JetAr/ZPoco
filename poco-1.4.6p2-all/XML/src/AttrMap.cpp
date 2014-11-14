﻿//
// AttrMap.cpp
//
// $Id: //poco/1.4/XML/src/AttrMap.cpp#1 $
//
// Library: XML
// Package: DOM
// Module:  DOM
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#include "Poco/DOM/AttrMap.h"
#include "Poco/DOM/Attr.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/DOMException.h"


namespace Poco
{
namespace XML
{


AttrMap::AttrMap(Element* pElement):
    _pElement(pElement)
{
    poco_check_ptr (pElement);

    _pElement->duplicate();
}


AttrMap::~AttrMap()
{
    _pElement->release();
}


Node* AttrMap::getNamedItem(const XMLString& name) const
{
    return _pElement->getAttributeNode(name);
}


Node* AttrMap::setNamedItem(Node* arg)
{
    poco_check_ptr (arg);

    if (arg->nodeType() != Node::ATTRIBUTE_NODE)
        throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);

    return _pElement->setAttributeNode(static_cast<Attr*>(arg));
}


Node* AttrMap::removeNamedItem(const XMLString& name)
{
    Attr* pAttr = _pElement->getAttributeNode(name);
    if (pAttr)
        return _pElement->removeAttributeNode(pAttr);
    else
        return 0;
}


Node* AttrMap::item(unsigned long index) const
{
    AbstractNode* pAttr = _pElement->_pFirstAttr;
    while (index-- > 0 && pAttr) pAttr = static_cast<AbstractNode*>(pAttr->nextSibling());
    return pAttr;
}


unsigned long AttrMap::length() const
{
    unsigned long result = 0;
    AbstractNode* pAttr = _pElement->_pFirstAttr;
    while (pAttr)
    {
        pAttr = static_cast<AbstractNode*>(pAttr->nextSibling());
        ++result;
    }
    return result;
}


Node* AttrMap::getNamedItemNS(const XMLString& namespaceURI, const XMLString& localName) const
{
    return _pElement->getAttributeNodeNS(namespaceURI, localName);
}


Node* AttrMap::setNamedItemNS(Node* arg)
{
    poco_check_ptr (arg);

    if (arg->nodeType() != Node::ATTRIBUTE_NODE)
        throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);

    return _pElement->setAttributeNodeNS(static_cast<Attr*>(arg));
}


Node* AttrMap::removeNamedItemNS(const XMLString& namespaceURI, const XMLString& localName)
{
    Attr* pAttr = _pElement->getAttributeNodeNS(namespaceURI, localName);
    if (pAttr)
        return _pElement->removeAttributeNode(pAttr);
    else
        return 0;
}


void AttrMap::autoRelease()
{
    _pElement->ownerDocument()->autoReleasePool().add(this);
}


}
} // namespace Poco::XML

