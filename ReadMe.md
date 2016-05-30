** License **

All of our code, i.e. code and assets created by the KWARC research group is lisenced under the MIT lisence. Any other code/assets are subject to their own lisences. Moreover, using this software requires you to accept the UNREAL® ENGINE END USER LICENSE AGREEMENT (https://www.unrealengine.com/eula).

The used vegetation pack is provided by the user "fighter5347" from the offical unreal forum. Find the official download at (https://forums.unrealengine.com/showthread.php?59812-FREE-Foliage-Starter-Kit) for the complete package.

** Directory Structure **

<Game-Folder>
    |--mmt
	|--Theories
    		|--urtheories
		|--frameit
	|--mmt.jar
	|--frameit-mmt.jar
	|--mmtrc
	|--startup.msl

Theories contains the archives

** Starting MMT **

run mmt (relative to the game-folder) with
	java -Xmx2048m -cp "mmt/mmt.jar:mmt/frameit-mmt.jar" info.kwarc.mmt.api.frontend.Run "$@" --keepalive

this will also build the frameit archive and start the server on port 10000

** Communicating with MMT **

there are two HTTP requests:

1. http://localhost:10000/:frameit/add 
with body <content><omdoc>SITUATIONTHEORY</omdoc><omdoc>VIEW</omdoc></content>

2. http://localhost:10000/:frameit/pushout?theory=<SOLUTION-THEORY-URI>
to compute the pushout


** Example Requests **

The pushout for our example is computed using
	http://localhost:10000/:frameit/pushout?theory=http://cds.omdoc.org/FrameIT?solution_theory

The POST request for adding the situation theory and view is:
	http://localhost:10000/:frameit/add
Body:
<content>
<omdoc xmlns="http://omdoc.org/ns" xmlns:om="http://www.openmath.org/OpenMath"><theory name="situation_theory" base="http://cds.omdoc.org/FrameIT" meta="http://cds.omdoc.org/FrameIT?planar_geometry"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#42.2.0:684.25.1"/></metadata><constant name="pA">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#224.11.1:236.11.13"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="Vec3D"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#229.11.6:233.11.10"/></metadata></om:OMS></om:OMOBJ></type>
       
       
     </constant><constant name="pB">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#238.12.1:250.12.13"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="Vec3D"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#243.12.6:247.12.10"/></metadata></om:OMS></om:OMOBJ></type>
       
       
     </constant><constant name="pC">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#252.13.1:264.13.13"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="Vec3D"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#257.13.6:261.13.10"/></metadata></om:OMS></om:OMOBJ></type>
       
       
     </constant><constant name="anglepApBpC_value">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#268.15.1:351.15.84"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMA>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="tm"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA></om:OMOBJ></type>
       <definition><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMLIT value="1.5707963267948966"><type><om:OMA>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="tm"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA></type></om:OMLIT></om:OMOBJ></definition>
       
     </constant><constant name="anglepApBpC">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#354.16.2:404.16.52"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#368.16.16:401.16.49"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="ded"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#370.16.18:401.16.49"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="equal"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#370.16.18:381.16.29"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="angle"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pA"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/planar_geometry.mmt#5555.141.55:5555.141.55"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pB"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/planar_geometry.mmt#5566.141.66:5566.141.66"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pC"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/planar_geometry.mmt#5569.141.69:5569.141.69"/></metadata></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="anglepApBpC_value"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#385.16.33:401.16.49"/></metadata></om:OMS>
      </om:OMA>
      </om:OMA></om:OMOBJ></type>
       
       
     </constant><constant name="anglepCpApB_value">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#420.18.2:450.18.32"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="realSym"></om:OMS></om:OMOBJ></type>
       <definition><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#484.19.33:500.19.49"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="div"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="pi"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#440.18.22:441.18.23"/></metadata></om:OMS><om:OMLIT value="4.0"><type><om:OMA>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="tm"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA></type></om:OMLIT>
      </om:OMA></om:OMOBJ></definition>
       
     </constant><constant name="anglepCpApB">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#453.19.2:503.19.52"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#467.19.16:500.19.49"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="ded"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#469.19.18:500.19.49"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="equal"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#469.19.18:480.19.29"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="angle"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pC"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#472.19.21:473.19.22"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pA"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#475.19.24:476.19.25"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pB"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#478.19.27:479.19.28"/></metadata></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="anglepCpApB_value"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#484.19.33:500.19.49"/></metadata></om:OMS>
      </om:OMA>
      </om:OMA></om:OMOBJ></type>
       
       
     </constant><constant name="lineSegpApB_value">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#509.21.2:534.21.27"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMA>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="tm"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA></om:OMOBJ></type>
       <definition><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMLIT value="100.0"><type><om:OMA>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="tm"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA></type></om:OMLIT></om:OMOBJ></definition>
       
     </constant><constant name="lineSegpApB">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#537.22.2:600.22.65"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#551.22.16:597.22.62"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="ded"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#553.22.18:597.22.62"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="equal"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#553.22.18:577.22.42"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="lineSegmentLength"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#554.22.19:570.22.35"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pA"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#572.22.37:573.22.38"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pB"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#575.22.40:576.22.41"/></metadata></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="lineSegpApB_value"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#581.22.46:597.22.62"/></metadata></om:OMS>
      </om:OMA>
      </om:OMA></om:OMOBJ></type>
       
       
     </constant><constant name="givenProof">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#611.24.1:681.24.71"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#624.24.14:650.24.40"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="ded"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#626.24.16:650.24.40"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="perp"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#627.24.17:636.24.26"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="pair"></om:OMS><om:OMA>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="cartesianProduct"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/planar_geometry.mmt#3145.81.22:3157.81.34"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="cartesianProduct"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA><om:OMA>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="cartesianProduct"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/planar_geometry.mmt#3145.81.22:3157.81.34"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="cartesianProduct"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pA"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#629.24.19:630.24.20"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pB"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#633.24.23:634.24.24"/></metadata></om:OMS>
      </om:OMA><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#640.24.30:649.24.39"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="pair"></om:OMS><om:OMA>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="cartesianProduct"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/planar_geometry.mmt#3145.81.22:3157.81.34"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="cartesianProduct"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA><om:OMA>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="cartesianProduct"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/planar_geometry.mmt#3145.81.22:3157.81.34"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="cartesianProduct"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pB"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#642.24.32:643.24.33"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pC"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#646.24.36:647.24.37"/></metadata></om:OMS>
      </om:OMA>
      </om:OMA>
      </om:OMA></om:OMOBJ></type>
       <definition><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#656.24.46:678.24.68"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="perp_axiom1"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pA"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/planar_geometry.mmt#5555.141.55:5555.141.55"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pB"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/planar_geometry.mmt#5566.141.66:5566.141.66"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pC"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/planar_geometry.mmt#5569.141.69:5569.141.69"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="anglepApBpC"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/situation_theory.mmt#668.24.58:678.24.68"/></metadata></om:OMS>
      </om:OMA></om:OMOBJ></definition>
       
     </constant></theory></omdoc><omdoc xmlns="http://omdoc.org/ns" xmlns:om="http://www.openmath.org/OpenMath"><view from="http://cds.omdoc.org/FrameIT?problem_theory" to="http://cds.omdoc.org/FrameIT?situation_theory" name="situation_problem_view" base="http://cds.omdoc.org/FrameIT">
           <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#42.2.0:388.13.1"/></metadata><constant name="[http://cds.omdoc.org/FrameIT?problem_theory]/Pa">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#112.3.1:121.3.10"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="Vec3D"></om:OMS></om:OMOBJ></type>
       <definition><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pA"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#117.3.6:118.3.7"/></metadata></om:OMS></om:OMOBJ></definition>
       
     </constant><constant name="[http://cds.omdoc.org/FrameIT?problem_theory]/Pb">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#124.4.2:133.4.11"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="Vec3D"></om:OMS></om:OMOBJ></type>
       <definition><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pB"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#129.4.7:130.4.8"/></metadata></om:OMS></om:OMOBJ></definition>
       
     </constant><constant name="[http://cds.omdoc.org/FrameIT?problem_theory]/Pc">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#136.5.2:145.5.11"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="Vec3D"></om:OMS></om:OMOBJ></type>
       <definition><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pC"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#141.5.7:142.5.8"/></metadata></om:OMS></om:OMOBJ></definition>
       
     </constant><constant name="[http://cds.omdoc.org/FrameIT?problem_theory]/anglePaPbPc_value">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#148.6.2:187.6.41"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="realSym"></om:OMS></om:OMOBJ></type>
       <definition><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="anglepApBpC_value"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#168.6.22:184.6.38"/></metadata></om:OMS></om:OMOBJ></definition>
       
     </constant><constant name="[http://cds.omdoc.org/FrameIT?problem_theory]/anglePaPbPc">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#190.7.2:217.7.29"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/problem_theory.mmt#311.16.15:344.16.48"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="ded"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/problem_theory.mmt#313.16.17:344.16.48"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="equal"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/problem_theory.mmt#313.16.17:324.16.28"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="angle"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pA"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#117.3.6:118.3.7"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pB"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#129.4.7:130.4.8"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pC"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#141.5.7:142.5.8"/></metadata></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="anglepApBpC_value"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#168.6.22:184.6.38"/></metadata></om:OMS>
      </om:OMA>
      </om:OMA></om:OMOBJ></type>
       <definition><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="anglepApBpC"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#204.7.16:214.7.26"/></metadata></om:OMS></om:OMOBJ></definition>
       
     </constant><constant name="[http://cds.omdoc.org/FrameIT?problem_theory]/anglePcPaPb_value">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#220.8.2:259.8.41"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="realSym"></om:OMS></om:OMOBJ></type>
       <definition><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="anglepCpApB_value"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#240.8.22:256.8.38"/></metadata></om:OMS></om:OMOBJ></definition>
       
     </constant><constant name="[http://cds.omdoc.org/FrameIT?problem_theory]/anglePcPaPb">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#262.9.2:289.9.29"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/problem_theory.mmt#397.19.16:430.19.49"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="ded"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/problem_theory.mmt#399.19.18:430.19.49"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="equal"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/problem_theory.mmt#399.19.18:410.19.29"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="angle"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pC"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#141.5.7:142.5.8"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pA"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#117.3.6:118.3.7"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pB"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#129.4.7:130.4.8"/></metadata></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="anglepCpApB_value"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#240.8.22:256.8.38"/></metadata></om:OMS>
      </om:OMA>
      </om:OMA></om:OMOBJ></type>
       <definition><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="anglepCpApB"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#276.9.16:286.9.26"/></metadata></om:OMS></om:OMOBJ></definition>
       
     </constant><constant name="[http://cds.omdoc.org/FrameIT?problem_theory]/lineSegPaPb_value">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#294.10.2:333.10.41"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="realSym"></om:OMS></om:OMOBJ></type>
       <definition><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="lineSegpApB_value"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#314.10.22:330.10.38"/></metadata></om:OMS></om:OMOBJ></definition>
       
     </constant><constant name="[http://cds.omdoc.org/FrameIT?problem_theory]/lineSegPaPb">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#336.11.2:363.11.29"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/problem_theory.mmt#483.22.16:529.22.62"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="ded"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/problem_theory.mmt#485.22.18:529.22.62"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="equal"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/problem_theory.mmt#485.22.18:509.22.42"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="lineSegmentLength"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pA"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#117.3.6:118.3.7"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pB"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#129.4.7:130.4.8"/></metadata></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="lineSegpApB_value"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#314.10.22:330.10.38"/></metadata></om:OMS>
      </om:OMA>
      </om:OMA></om:OMOBJ></type>
       <definition><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="lineSegpApB"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#350.11.16:360.11.26"/></metadata></om:OMS></om:OMOBJ></definition>
       
     </constant><constant name="[http://cds.omdoc.org/FrameIT?problem_theory]/proof">
       <metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#366.12.2:386.12.22"/></metadata>
       <type><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/problem_theory.mmt#545.24.10:571.24.36"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="HOL" name="ded"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/problem_theory.mmt#547.24.12:571.24.36"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="perp"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/problem_theory.mmt#548.24.13:557.24.22"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="pair"></om:OMS><om:OMA>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="cartesianProduct"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/planar_geometry.mmt#3145.81.22:3157.81.34"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="cartesianProduct"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA><om:OMA>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="cartesianProduct"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/planar_geometry.mmt#3145.81.22:3157.81.34"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="cartesianProduct"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pA"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#117.3.6:118.3.7"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pB"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#129.4.7:130.4.8"/></metadata></om:OMS>
      </om:OMA><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/problem_theory.mmt#561.24.26:570.24.35"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="pair"></om:OMS><om:OMA>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="cartesianProduct"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/planar_geometry.mmt#3145.81.22:3157.81.34"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="cartesianProduct"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA><om:OMA>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="cartesianProduct"></om:OMS><om:OMA><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/planar_geometry.mmt#3145.81.22:3157.81.34"/></metadata>
              <om:OMS base="http://cds.omdoc.org/urtheories" module="LambdaPi" name="apply"></om:OMS>
              <om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="cartesianProduct"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="planar_geometry" name="reals"></om:OMS>
      </om:OMA><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pB"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#129.4.7:130.4.8"/></metadata></om:OMS><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="pC"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#141.5.7:142.5.8"/></metadata></om:OMS>
      </om:OMA>
      </om:OMA>
      </om:OMA></om:OMOBJ></type>
       <definition><om:OMOBJ xmlns:om="http://www.openmath.org/OpenMath"><om:OMS base="http://cds.omdoc.org/FrameIT" module="situation_theory" name="givenProof"><metadata><link rel="http://cds.omdoc.org/mmt?metadata?sourceRef" resource="http://docs.omdoc.org/FrameIT/frameit_view.mmt#374.12.10:383.12.19"/></metadata></om:OMS></om:OMOBJ></definition>
       
     </constant>
         </view></omdoc>
</content>

