# **Flight Computer Software Engineering Charter v1.0**

## **Mission**

This codebase exists to produce deterministic, reliable, high-performance software for safety-critical and mission-critical systems.

Every line of code must prioritize:

1. Correctness  
2. Determinism  
3. Performance  
4. Maintainability  
5. Testability

No optimization is worth undefined behavior.

---

# **Core Principles**

## **1\. Correctness before speed**

Performance only matters after correctness is proven.

Never sacrifice correctness for benchmark numbers.

Every optimization must preserve behavior.

---

## **2\. Deterministic execution**

The software shall behave identically for identical inputs.

Avoid:

* hidden state  
* timing-dependent behavior  
* non-deterministic algorithms  
* random ordering  
* race conditions

Determinism is mandatory.

---

## **3\. Zero Undefined Behavior**

The codebase shall compile warning-free.

Undefined behavior is treated as a defect.

Examples include

* signed integer overflow  
* invalid pointer arithmetic  
* use-after-free  
* data races  
* uninitialized memory  
* dangling references  
* strict aliasing violations

If behavior is not defined by the C++ standard, it is forbidden.

---

## **4\. Performance is predictable**

Low average latency is not enough.

Worst-case execution time is the metric that matters.

Software shall minimize

* cache misses  
* memory fragmentation  
* branch mispredictions  
* dynamic allocation  
* unpredictable execution paths

---

# **Memory Rules**

Dynamic allocation is forbidden during mission execution.

Memory shall be allocated during initialization only.

No:

* new  
* delete  
* malloc  
* free

inside real-time loops.

Memory ownership must always be obvious.

Prefer

* std::array  
* stack allocation  
* fixed-size pools  
* arena allocators

---

# **AI Usage Policy**

AI is an assistant.

AI is never an authority.

Every AI-generated line of code shall be reviewed before acceptance.

AI may assist with

* boilerplate  
* documentation  
* unit tests  
* refactoring suggestions

AI shall never be trusted to

* implement control algorithms without review  
* verify mathematical correctness  
* prove safety  
* make architectural decisions independently

Generated code must satisfy the same standards as handwritten code.

---

# **Testing Requirements**

Every module shall include tests.

Minimum:

* Unit Tests  
* Integration Tests  
* Stress Tests  
* Boundary Tests  
* Regression Tests

Every discovered bug shall produce a regression test before the fix is merged.

A bug without a regression test is considered unresolved.

---

# **Numerical Safety**

Floating point behavior must be intentional.

Avoid equality comparisons.

Validate

* overflow  
* underflow  
* NaN  
* infinities

Critical calculations must document

* precision requirements  
* acceptable error bounds  
* numerical assumptions

---

# **Error Handling**

Software shall fail safely.

Never ignore errors.

Every failure shall

* return status  
* log context  
* preserve system integrity

Exceptions are prohibited in real-time code.

Use explicit error handling.

---

# **Concurrency**

No data races.

Synchronization must be explicit.

Shared mutable state shall be minimized.

Lock-free structures may only be used after correctness is proven.

Priority inversion must be considered during design.

---

# **Code Review**

Every change requires review.

Review checklist:

* correctness  
* safety  
* determinism  
* readability  
* performance  
* test coverage

No code merges without approval.

---

# **Static Analysis**

Every commit shall pass

* compiler warnings at highest practical level  
* static analyzers  
* formatting  
* linting  
* unit tests

Warnings are treated as errors.

---

# **Coding Style**

Readable code is safer code.

Functions should do one thing.

Prefer descriptive names over comments.

Avoid clever code.

Complexity must be justified.

---

# **Architecture**

Modules shall have

* single responsibility  
* minimal coupling  
* explicit interfaces

Hardware interaction shall be isolated behind interfaces.

Mission logic shall never directly manipulate hardware registers.

---

# **Documentation**

Every subsystem shall document

* purpose  
* assumptions  
* timing constraints  
* memory usage  
* failure modes  
* dependencies

Complex algorithms shall include references to papers or specifications.

---

# **Version Control**

Every commit must

* compile  
* pass all tests  
* include meaningful commit messages

Broken builds are unacceptable.

---

# **Real-Time Constraints**

Every periodic task shall specify

* frequency  
* deadline  
* worst-case execution time  
* stack usage

Missing a deadline is a software defect.

---

# **Safety Requirements**

Critical decisions shall never rely on a single sensor when redundancy exists.

All external inputs must be validated.

Impossible states should be unrepresentable where practical.

Assertions shall verify developer assumptions during testing.

Production code shall recover gracefully from expected failures.

---

# **Build Rules**

Builds must be reproducible.

Compiler version, flags, dependencies, and generated binaries shall be traceable.

No undocumented compiler flags.

---

# **Quality Gates**

Code cannot be merged unless it:

* Compiles with zero warnings.  
* Passes all automated tests.  
* Passes static analysis.  
* Has measurable test coverage.  
* Meets timing requirements.  
* Has no known memory leaks.  
* Has documented public interfaces.  
* Is reviewed by another engineer.

---

# **Engineering Philosophy**

Every line of code controls real hardware.

Every bug has a cost.

Every optimization must be measurable.

Every assumption must be verified.

Simplicity is preferred over cleverness.

Correctness is preferred over speed.

Determinism is preferred over convenience.

Safety is never negotiable.

# **4.1 MISRA C++ Compliance**

## **Purpose**

The codebase shall adhere to the principles of MISRA C++ to reduce ambiguity, undefined behavior, and unsafe language constructs.

## **Requirements**

* All production code shall comply with the selected MISRA C++ ruleset unless a documented deviation is approved.  
* Every deviation shall include:  
  * Rule identifier.  
  * Technical justification.  
  * Risk assessment.  
  * Mitigation strategy.  
  * Reviewer approval.  
* No undocumented deviations are permitted.  
* Compliance shall be verified using static analysis tools integrated into CI.

---

# **4.2 AUTOSAR C++ Guidelines**

## **Purpose**

AUTOSAR C++ rules shall supplement the coding standard for modern C++ usage where they improve safety, maintainability, and correctness.

## **Requirements**

* Approved AUTOSAR C++ rules shall be followed for language features, object lifetime, ownership, templates, and API design.  
* Any conflicts between AUTOSAR and project-specific requirements shall be documented and resolved through formal review.  
* Compliance shall be checked automatically where tooling is available.

---

# **4.3 Requirements Traceability**

## **Purpose**

Every implemented behavior shall be traceable to an approved requirement.

## **Requirements**

Every requirement shall possess:

* Unique identifier.  
* Description.  
* Priority.  
* Safety classification.  
* Verification method.

Each requirement shall be linked to:

* Design documentation.  
* Source code modules.  
* Unit tests.  
* Integration tests.  
* System tests.

No code shall exist without an associated requirement.

No requirement shall exist without verification.

The traceability chain shall remain complete:

Requirement  
      ↓  
Architecture  
      ↓  
Design  
      ↓  
Implementation  
      ↓  
Unit Tests  
      ↓  
Integration Tests  
      ↓  
System Validation  
---

# **4.4 Verification Requirements**

## **Purpose**

Every requirement shall be objectively verified.

## **Requirements**

Each requirement shall define one or more verification methods:

* Unit Test  
* Integration Test  
* Hardware Test  
* Analysis  
* Inspection  
* Simulation

Verification shall be automated whenever practical.

Manual verification shall require documented evidence.

Regression tests shall be added for every corrected defect.

---

# **4.5 Code Coverage Policy**

## **Purpose**

Testing shall demonstrate that implemented logic has been exercised.

## **Requirements**

Minimum coverage goals:

* Statement Coverage: 100%  
* Branch Coverage: 100%  
* Function Coverage: 100%

For safety-critical modules:

* Modified Condition/Decision Coverage (MC/DC) shall be achieved where applicable.

Coverage reports shall be generated automatically.

Reduced coverage shall require documented justification.

Coverage metrics shall never replace meaningful test quality.

---

# **4.6 Code Review Policy**

## **Purpose**

Independent review reduces systematic defects.

## **Requirements**

Every merge request shall receive review.

Changes affecting:

* Flight control  
* Navigation  
* Sensor fusion  
* Guidance  
* Mission logic  
* Safety systems

shall require approval from at least two qualified reviewers.

Reviewers shall evaluate:

* Correctness  
* Safety  
* Performance  
* Determinism  
* Maintainability  
* Compliance  
* Test quality

Self-approval is prohibited.

---

# **4.7 Verification Levels**

## **Purpose**

Software shall be validated in progressively realistic environments.

## **Requirements**

Every release shall pass:

### **Software-in-the-Loop (SIL)**

* Algorithm correctness  
* Logic verification  
* Numerical validation  
* Automated regression suite

### **Processor-in-the-Loop (PIL)**

* Target CPU verification  
* Compiler verification  
* Timing validation  
* Memory validation

### **Hardware-in-the-Loop (HIL)**

* Sensor simulation  
* Actuator interfaces  
* Communication buses  
* Real-time scheduling  
* Fault injection  
* Timing behavior

Production releases shall not bypass these verification stages.

---

# **4.8 Flight-Critical Language Restrictions**

## **Purpose**

Certain language features introduce unacceptable runtime uncertainty.

## **Prohibited Features**

Flight-critical code shall not use:

* Exceptions  
* RTTI  
* Dynamic casts  
* Heap allocation during operation  
* Garbage collection  
* Recursion  
* Global mutable state  
* Undefined behavior  
* Compiler-specific extensions without approval

Template metaprogramming shall remain understandable and measurable.

constexpr shall be preferred where appropriate.

---

# **4.9 Integer and Data Type Policy**

## **Purpose**

Hardware interfaces require deterministic binary representations.

## **Requirements**

Hardware-facing interfaces shall exclusively use fixed-width integer types.

Examples:

std::uint8\_t  
std::uint16\_t  
std::uint32\_t  
std::uint64\_t

std::int8\_t  
std::int16\_t  
std::int32\_t  
std::int64\_t

The following shall not appear in hardware interfaces:

int  
long  
short  
unsigned

Bitfields shall not be used for portable interfaces unless fully documented.

Endianness assumptions shall be explicit.

Overflow behavior shall be documented.

---

# **4.10 Algorithm Documentation Standard**

## **Purpose**

Every algorithm shall be understandable, reviewable, and analyzable.

## **Every algorithm shall include:**

### **Purpose**

Describe the algorithm's objective.

---

### **Inputs**

Document all inputs.

Units shall be specified.

Expected ranges shall be specified.

---

### **Outputs**

Document outputs.

Units.

Precision.

Error tolerance.

---

### **Computational Complexity**

Example:

Time Complexity

O(n log n)

Worst Case

O(n²)

Memory Complexity

O(n)  
---

### **Worst-Case Execution Time (WCET)**

Measured on target hardware.

Example:

Average

1.8 ms

Worst Case

2.4 ms

Deadline

5 ms  
---

### **Memory Usage**

Document:

Static RAM

48 KB

Stack

6 KB

Flash

22 KB

Dynamic allocation shall be explicitly prohibited if applicable.

---

### **Numerical Assumptions**

Document:

* Floating-point precision  
* Rounding strategy  
* Overflow assumptions  
* NaN handling  
* Saturation behavior  
* Coordinate frames  
* Units

---

### **Failure Modes**

Every algorithm shall document:

Possible failures

Example:

* Sensor unavailable  
* GPS timeout  
* Divide by zero  
* Overflow  
* Invalid quaternion  
* Matrix singularity

Mitigation

* Reject measurement  
* Switch estimator  
* Reset filter  
* Enter degraded mode  
* Notify fault manager

---

### **Verification**

Document:

* Unit tests  
* Integration tests  
* SIL verification  
* HIL verification  
* Performance benchmarks  
* Fault injection results

---

# **4.11 Worst-Case Execution Time (WCET) Analysis**

## **Purpose**

Real-time guarantees require analysis of the maximum execution time rather than average performance.

## **Requirements**

* Every periodic task shall have a documented WCET measured on target hardware.  
* WCET shall include cache effects, branch behavior, interrupt latency, and compiler optimizations where relevant.  
* The sum of WCETs for all scheduled tasks shall not exceed the available CPU budget with an appropriate safety margin.  
* Any change that increases WCET beyond the allocated budget shall require review and revalidation.

