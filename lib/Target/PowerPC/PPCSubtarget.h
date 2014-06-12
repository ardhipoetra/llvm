//===-- PPCSubtarget.h - Define Subtarget for the PPC ----------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the PowerPC specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef POWERPCSUBTARGET_H
#define POWERPCSUBTARGET_H

#include "PPCFrameLowering.h"
#include "llvm/ADT/Triple.h"
#include "llvm/MC/MCInstrItineraries.h"
#include "llvm/Target/TargetSubtargetInfo.h"
#include <string>

#define GET_SUBTARGETINFO_HEADER
#include "PPCGenSubtargetInfo.inc"

// GCC #defines PPC on Linux but we use it as our namespace name
#undef PPC

namespace llvm {
class StringRef;

namespace PPC {
  // -m directive values.
  enum {
    DIR_NONE,
    DIR_32,
    DIR_440,
    DIR_601,
    DIR_602,
    DIR_603,
    DIR_7400,
    DIR_750,
    DIR_970,
    DIR_A2,
    DIR_E500mc,
    DIR_E5500,
    DIR_PWR3,
    DIR_PWR4,
    DIR_PWR5,
    DIR_PWR5X,
    DIR_PWR6,
    DIR_PWR6X,
    DIR_PWR7,
    DIR_64
  };
}

class GlobalValue;
class TargetMachine;

class PPCSubtarget : public PPCGenSubtargetInfo {
protected:
  /// stackAlignment - The minimum alignment known to hold of the stack frame on
  /// entry to the function and which must be maintained by every function.
  unsigned StackAlignment;

  /// Selected instruction itineraries (one entry per itinerary class.)
  InstrItineraryData InstrItins;

  /// Which cpu directive was used.
  unsigned DarwinDirective;

  /// Used by the ISel to turn in optimizations for POWER4-derived architectures
  bool HasMFOCRF;
  bool Has64BitSupport;
  bool Use64BitRegs;
  bool UseCRBits;
  bool IsPPC64;
  bool HasAltivec;
  bool HasQPX;
  bool HasVSX;
  bool HasFCPSGN;
  bool HasFSQRT;
  bool HasFRE, HasFRES, HasFRSQRTE, HasFRSQRTES;
  bool HasRecipPrec;
  bool HasSTFIWX;
  bool HasLFIWAX;
  bool HasFPRND;
  bool HasFPCVT;
  bool HasISEL;
  bool HasPOPCNTD;
  bool HasLDBRX;
  bool IsBookE;
  bool DeprecatedMFTB;
  bool DeprecatedDST;
  bool HasLazyResolverStubs;
  bool IsJITCodeModel;
  bool IsLittleEndian;

  /// TargetTriple - What processor and OS we're targeting.
  Triple TargetTriple;

  /// OptLevel - What default optimization level we're emitting code for.
  CodeGenOpt::Level OptLevel;

  PPCFrameLowering    FrameLowering;
public:
  /// This constructor initializes the data members to match that
  /// of the specified triple.
  ///
  PPCSubtarget(const std::string &TT, const std::string &CPU,
               const std::string &FS, bool is64Bit,
               CodeGenOpt::Level OptLevel);

  /// ParseSubtargetFeatures - Parses features string setting specified
  /// subtarget options.  Definition of function is auto generated by tblgen.
  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);

  /// SetJITMode - This is called to inform the subtarget info that we are
  /// producing code for the JIT.
  void SetJITMode();

  /// getStackAlignment - Returns the minimum alignment known to hold of the
  /// stack frame on entry to the function and which must be maintained by every
  /// function for this subtarget.
  unsigned getStackAlignment() const { return StackAlignment; }

  /// getDarwinDirective - Returns the -m directive specified for the cpu.
  ///
  unsigned getDarwinDirective() const { return DarwinDirective; }

  /// getInstrItins - Return the instruction itineraies based on subtarget
  /// selection.
  const InstrItineraryData &getInstrItineraryData() const { return InstrItins; }

  const PPCFrameLowering *getFrameLowering() const { return &FrameLowering; }

  /// initializeSubtargetDependencies - Initializes using a CPU and feature string
  /// so that we can use initializer lists for subtarget initialization.
  PPCSubtarget &initializeSubtargetDependencies(StringRef CPU, StringRef FS);

  /// \brief Reset the features for the PowerPC target.
  void resetSubtargetFeatures(const MachineFunction *MF) override;
private:
  void initializeEnvironment();
  void resetSubtargetFeatures(StringRef CPU, StringRef FS);

public:
  /// isPPC64 - Return true if we are generating code for 64-bit pointer mode.
  ///
  bool isPPC64() const { return IsPPC64; }

  /// has64BitSupport - Return true if the selected CPU supports 64-bit
  /// instructions, regardless of whether we are in 32-bit or 64-bit mode.
  bool has64BitSupport() const { return Has64BitSupport; }

  /// use64BitRegs - Return true if in 64-bit mode or if we should use 64-bit
  /// registers in 32-bit mode when possible.  This can only true if
  /// has64BitSupport() returns true.
  bool use64BitRegs() const { return Use64BitRegs; }

  /// useCRBits - Return true if we should store and manipulate i1 values in
  /// the individual condition register bits.
  bool useCRBits() const { return UseCRBits; }

  /// hasLazyResolverStub - Return true if accesses to the specified global have
  /// to go through a dyld lazy resolution stub.  This means that an extra load
  /// is required to get the address of the global.
  bool hasLazyResolverStub(const GlobalValue *GV,
                           const TargetMachine &TM) const;

  // isJITCodeModel - True if we're generating code for the JIT
  bool isJITCodeModel() const { return IsJITCodeModel; }

  // isLittleEndian - True if generating little-endian code
  bool isLittleEndian() const { return IsLittleEndian; }

  // Specific obvious features.
  bool hasFCPSGN() const { return HasFCPSGN; }
  bool hasFSQRT() const { return HasFSQRT; }
  bool hasFRE() const { return HasFRE; }
  bool hasFRES() const { return HasFRES; }
  bool hasFRSQRTE() const { return HasFRSQRTE; }
  bool hasFRSQRTES() const { return HasFRSQRTES; }
  bool hasRecipPrec() const { return HasRecipPrec; }
  bool hasSTFIWX() const { return HasSTFIWX; }
  bool hasLFIWAX() const { return HasLFIWAX; }
  bool hasFPRND() const { return HasFPRND; }
  bool hasFPCVT() const { return HasFPCVT; }
  bool hasAltivec() const { return HasAltivec; }
  bool hasQPX() const { return HasQPX; }
  bool hasVSX() const { return HasVSX; }
  bool hasMFOCRF() const { return HasMFOCRF; }
  bool hasISEL() const { return HasISEL; }
  bool hasPOPCNTD() const { return HasPOPCNTD; }
  bool hasLDBRX() const { return HasLDBRX; }
  bool isBookE() const { return IsBookE; }
  bool isDeprecatedMFTB() const { return DeprecatedMFTB; }
  bool isDeprecatedDST() const { return DeprecatedDST; }

  const Triple &getTargetTriple() const { return TargetTriple; }

  /// isDarwin - True if this is any darwin platform.
  bool isDarwin() const { return TargetTriple.isMacOSX(); }
  /// isBGQ - True if this is a BG/Q platform.
  bool isBGQ() const { return TargetTriple.getVendor() == Triple::BGQ; }

  bool isDarwinABI() const { return isDarwin(); }
  bool isSVR4ABI() const { return !isDarwin(); }

  /// enablePostRAScheduler - True at 'More' optimization.
  bool enablePostRAScheduler(CodeGenOpt::Level OptLevel,
                             TargetSubtargetInfo::AntiDepBreakMode& Mode,
                             RegClassVector& CriticalPathRCs) const override;

  bool enableEarlyIfConversion() const override { return hasISEL(); }

  // Scheduling customization.
  bool enableMachineScheduler() const override;
  void overrideSchedPolicy(MachineSchedPolicy &Policy,
                           MachineInstr *begin,
                           MachineInstr *end,
                           unsigned NumRegionInstrs) const override;
  bool useAA() const override;
};
} // End llvm namespace

#endif
