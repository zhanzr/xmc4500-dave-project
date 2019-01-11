<?xml version="1.0" encoding="ASCII"?>
<ResourceModel:App xmi:version="2.0" xmlns:xmi="http://www.omg.org/XMI" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:ResourceModel="http://www.infineon.com/Davex/Resource.ecore" name="FCE_CONFIG" URI="http://resources/4.0.6/app/FCE_CONFIG/0" description="The FCE_CONFIG provides a parallel implementation of Cyclic Redundancy Code (CRC) algorithms which is used as a hardware acceleration engine for software applications" mode="NOTSHARABLE" version="4.0.6" minDaveVersion="4.1.2" instanceLabel="FCE_CONFIG_0" appLabel="">
  <properties provideInit="true"/>
  <virtualSignals name="fce_sr0" URI="http://resources/4.0.6/app/FCE_CONFIG/0/vs_fce_srv_request" hwSignal="int" hwResource="//@hwResources.0" visible="true"/>
  <hwResources name="FCE_CONFIG" URI="http://resources/4.0.6/app/FCE_CONFIG/0/hwres_fce_slice" resourceGroupUri="peripheral/fce/0/kernel/sv0" solverVariable="true" mResGrpUri="peripheral/fce/0/kernel/sv0">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/FCE/FCE_0.dd#//@provided.0"/>
    <solverVarMap index="4">
      <value variableName="sv0" solverValue="3"/>
    </solverVarMap>
    <solverVarMap index="4">
      <value variableName="sv0" solverValue="3"/>
    </solverVarMap>
  </hwResources>
</ResourceModel:App>
