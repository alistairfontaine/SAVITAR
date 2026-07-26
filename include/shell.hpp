#ifndef SHELL_HPP
#define SHELL_HPP

#include "savitar.hpp"

namespace Savitar {

/**
 * 💻 THE OFFICIAL INTERACTIVE TERMINAL SHELL LOOP 💻
 * Hooks directly into standard input/output streams to spin an ultra-minimalist CLI workspace.
 * Parses user command strings and maps inputs straight onto low-level kernel optimization methods.
 */
void launch_interactive_shell(FaultIsolationKernel& kernel);

} // namespace Savitar

#endif // SHELL_HPP
