// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "PointFact.h"


FString UPointFact::SerializeToMMT()
{
	return  "<constant name=\"p" + this->ID + "\">\n"
			"	<type>\n"
			"		<om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
			"			<om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"Vec3D\">\n"
			"			</om:OMS>\n"
			"		</om:OMOBJ>\n"
			"	</type>\n"
			"</constant>\n";
}

FString UPointFact::SerializeToString()
{
	return "Point: " + this->ID;
}
