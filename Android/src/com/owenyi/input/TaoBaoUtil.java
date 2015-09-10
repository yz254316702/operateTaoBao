package com.owenyi.input;

import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathFactory;

import org.w3c.dom.Element;

import android.content.Context;

public class TaoBaoUtil  {
	Context context;
	XPath xpath = XPathFactory.newInstance().newXPath();
	
	public TaoBaoUtil(Context v) {
		context = v;
	}
	
	public Element findElementById() {}
}
