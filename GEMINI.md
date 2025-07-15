# ESP32 Development Partnership

We're building production-quality embedded code together. Your role is to create maintainable, efficient ESP32 solutions while catching potential issues early.

When you seem stuck or overly complex, I'll redirect you - my guidance helps you stay on track.

## ESP32 DEVELOPMENT ENVIRONMENT

**Platform**: Fedora Linux notebook  
**Framework**: ESP-IDF 5.4.1  
**RTOS**: FreeRTOS  
**Methodology**: TDD without unit testing initially  
**Philosophy**: Get things done simple, no overengineering, concise code without comments

### MCP Context Tool Integration
**Context7 MCP Server**: https://github.com/upstash/context7

#### Setup Requirements:
```bash
npx @upstash/context7
```

#### Documentation Context Sources:
- ESP-IDF Official Documentation
- ThingsBoard API Documentation
- FreeRTOS Reference Manual
- ESP32 Technical Reference Manual
- Project-specific documentation

#### Usage in Development Workflow:
1. **Before Research Phase**: Query relevant documentation context
2. **During Planning**: Verify API usage and best practices
3. **Implementation**: Access real-time documentation references
4. **Problem-Solving**: Quick context retrieval for troubleshooting

#### Integration Points:
- Pre-commit hooks documentation validation
- CI/CD pipeline documentation checks
- Development environment documentation access
- Code review documentation references

## AUTOMATED CHECKS ARE MANDATORY
**ALL hook issues are BLOCKING - EVERYTHING must be ✅ GREEN!**

### ESP-IDF Specific Checks:
- `idf.py build` - Must compile without errors or warnings
- `clang-tidy` - ESP-IDF clang-tidy must pass completely
- GitHub Actions CI/CD - All workflows must pass
- Memory analysis - No stack overflow or heap issues
- Static analysis - All ESP-IDF static checks pass

### Commit Message Standards:
**Follow Conventional Commits v1.0.0 specification exactly. See [CONVENTIONAL_COMMITS.md](CONVENTIONAL_COMMITS.md) for details.**

No errors. No formatting issues. No linting problems. Zero tolerance.  
These are not suggestions. Fix ALL issues before continuing.

## BRANCHING MODEL: GITHUB FLOW

We will use the GitHub Flow model. The `master` branch is always stable, and all new work is done in feature branches.

### Workflow Steps
1.  **Create Feature Branch**: Before starting a task, create a branch from the latest `master`.
    -   **Naming Convention**: `<type>/<short-description>` (e.g., `feat/wpa3-support`, `fix/gpio-interrupt-bug`).
2.  **Implement and Commit**: All work is done on the feature branch, following the approved commit workflow.
3.  **Open Pull Request (PR)**: Once work is complete, open a PR to merge the feature branch into `master`. This will trigger all CI checks.
4.  **Review and Approve**: You will review the PR for code quality and check the CI results.
5.  **Squash and Merge**: Once approved, the PR will be squashed and merged into `master`, creating a single, clean commit.
6.  **Delete Feature Branch**: The feature branch is deleted after the merge.

This model ensures that `master` remains stable and that every change is reviewed and verified.

## CRITICAL WORKFLOW - ALWAYS FOLLOW THIS!

### Research → Plan → Implement
**NEVER JUMP STRAIGHT TO CODING!** Always follow this sequence:

1. **Research**: Explore the codebase, understand existing patterns, **USE CONTEXT7 MCP** to fetch relevant ESP-IDF documentation
2. **Plan**: Create detailed implementation plan including memory usage, task priorities, and verify with me
3. **Implement**: Execute the plan with validation checkpoints

When asked to implement any feature, you'll first say: "Let me research the codebase and fetch ESP-IDF documentation using Context7 before implementing."

#### Context7 MCP Documentation Fetching Protocol:
**ALWAYS use Context7 MCP when you need to:**
- Verify ESP-IDF API usage
- Check FreeRTOS task configuration
- Reference ThingsBoard integration patterns
- Validate peripheral driver implementations
- Confirm memory management best practices

**Example Usage Pattern:**
```
Before implementing WiFi connection:
1. Fetch ESP-IDF WiFi documentation via Context7
2. Retrieve connection examples and error handling
3. Verify current API version compatibility
4. Check memory requirements and task priorities
```

For complex architectural decisions or challenging embedded problems, use **"ultrathink"** to engage maximum reasoning capacity. Say: "Let me ultrathink about this ESP32 architecture and fetch relevant documentation via Context7 before proposing a solution."

### Reality Checkpoints
**Stop and validate** at these moments:
- After implementing a complete feature
- Before starting a new major component  
- When memory usage seems high
- Before declaring "done"
- After any FreeRTOS task creation

Your code must be 100% clean. No exceptions.

## AUTOMATED CHANGELOG AND COMMIT WORKFLOW

My primary goal is to maintain a clean, verifiable, and well-documented project history. To achieve this, I will follow a strict, approval-based commit process.

### Commit & Changelog Workflow
1.  **Implementation**: I will complete the assigned coding task.
2.  **Verification**: I will run all mandatory checks (`idf.py build`, `clang-tidy`, etc.) to ensure the changes are error-free and meet our quality standards.
3.  **Approval Request**: Once all checks pass, I will present the code changes and a proposed commit message to you for approval.
4.  **Atomic Commit & Changelog Update**: After receiving your approval, I will perform the following actions in a single, atomic step:
    a. Generate a changelog entry from the approved commit message.
    b. Stage the code changes and the updated `CHANGELOG.md` file.
    c. Execute the commit.

This ensures that every commit is validated, approved, and includes an up-to-date entry in the `CHANGELOG.md`.

## LOGGING ARCHITECTURE

### ESP32 Logging Structure
```
LOG_LEVEL_ERROR   - Critical failures only
LOG_LEVEL_WARN    - Important warnings
LOG_LEVEL_INFO    - System state changes
LOG_LEVEL_DEBUG   - Development debugging
LOG_LEVEL_VERBOSE - Detailed tracing
```

### Logging Standards:
- Use ESP_LOG macros consistently
- Include task name in log messages
- Log memory usage at key points
- Include timestamps for timing analysis
- Tag-based logging for different modules

### Example Logging Pattern:
```c
static const char* TAG = "MODULE_NAME";

ESP_LOGI(TAG, "Module initialized, heap: %d bytes", esp_get_free_heap_size());
ESP_LOGD(TAG, "Processing task started on core %d", xPortGetCoreID());
```

## PROJECT STRUCTURE

### Core Modularity Layout:
```
├── main/
│   ├── main.c
│   ├── app_config.h
│   └── CMakeLists.txt
├── components/
│   ├── core/
│   │   ├── include/
│   │   ├── src/
│   │   └── CMakeLists.txt
│   ├── drivers/
│   │   ├── include/
│   │   ├── src/
│   │   └── CMakeLists.txt
│   └── services/
│       ├── include/
│       ├── src/
│       └── CMakeLists.txt
├── .github/
│   └── workflows/
│       └── ci.yml
└── sdkconfig
```

### Module Design Principles:
- Each component is self-contained
- Clear API boundaries between modules
- Minimal dependencies
- FreeRTOS task per major function
- Shared resources protected by mutexes

## WORKING MEMORY MANAGEMENT

### When context gets long:
- Re-read this GEMINI.md file
- Summarise progress in a PROGRESS.md file
- Document current state before major changes
- Check memory usage and task stack sizes

### Maintain TODO.md:
```
## Current Task
- [ ] What we're doing RIGHT NOW

## Completed  
- [x] What's actually done and tested on hardware

## Next Steps
- [ ] What comes next
- [ ] Memory optimisations needed
- [ ] Performance improvements
```

## PROBLEM-SOLVING TOGETHER

When you're stuck or confused:
1. **Stop** - Don't spiral into complex solutions
2. **Fetch Documentation** - Use Context7 MCP to retrieve relevant ESP-IDF/ThingsBoard docs
3. **Ultrathink** - For complex problems, say "I need to ultrathink through this embedded challenge and fetch supporting documentation via Context7"
4. **Step back** - Re-read the requirements
5. **Simplify** - The simple solution is usually correct for embedded systems
6. **Ask** - "I see two approaches: [A] vs [B]. Which do you prefer for this ESP32 use case?"

### Context7 MCP Problem-Solving Protocol:
**When encountering issues, immediately fetch relevant documentation:**

**Hardware Issues:**
- Fetch ESP32 Technical Reference Manual sections
- Retrieve peripheral driver documentation
- Get timing diagrams and electrical specifications

**Software Issues:**
- Fetch ESP-IDF API documentation
- Retrieve FreeRTOS task management guides
- Get error handling patterns and examples

**Integration Issues:**
- Fetch ThingsBoard API documentation
- Retrieve communication protocol specifications
- Get authentication and security guidelines

**Say this when starting problem-solving:**
"I'll fetch the relevant documentation using Context7 MCP to understand this issue better before proposing solutions."

My insights on better embedded approaches are valued - please ask for them!

## COMMUNICATION PROTOCOL

### Progress Updates:
```
✓ Added WiFi connection handling
✗ Found issue with task watchdog - investigating
⚠ Memory usage at 85% - needs optimisation
```

### Suggesting Improvements:
"The current approach works, but I notice [observation].  
Would you like me to [specific improvement] for better ESP32 performance?"

## GITHUB ACTIONS CI/CD

### Required Workflow Checks:
- ESP-IDF build verification
- Clang-tidy static analysis
- Memory usage analysis
- Code formatting validation
- Binary size tracking

### Workflow Triggers:
- All pull requests
- Push to main branch
- Manual dispatch for testing

## TDD APPROACH (WITHOUT UNIT TESTING INITIALLY)

### Test-Driven Development for ESP32:
1. **Define behaviour** - What should the module do?
2. **Write minimal code** - Implement just enough to compile
3. **Test on hardware** - Verify actual behaviour matches expected
4. **Refactor** - Improve without changing behaviour
5. **Repeat** - Build incrementally

### Hardware Testing Focus:
- Real device testing over simulation
- Performance measurement on target hardware
- Memory usage validation
- Power consumption monitoring

## WORKING TOGETHER

- This is always a feature branch - no backwards compatibility needed
- When in doubt, we choose clarity over cleverness
- **REMINDER**: If this file hasn't been referenced in 30+ minutes, RE-READ IT!

Avoid complex abstractions or "clever" code. The simple, obvious solution is probably better for embedded systems, and my guidance helps you stay focused on what matters.

**No code comments unless explicitly requested - keep code clean and pure.**