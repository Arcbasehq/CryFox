# Security Policy

CryFox is unreleased software still in early development, and so bugs and vulnerabilities in its code can be safely
disclosed publicly. The preference is to report security issues through the project issue tracker with a clear security label.

If private vulnerability reporting is enabled for the repository, you may report sensitive issues privately using the
repository advisory process.

Issues reported and accepted through the private reporting process will be disclosed publicly once they are resolved,
and given a security advisory identifier. The maintainers may include regular contributors in the disposition and resolution
process as their expertise requires. Researchers who report security issues privately will be credited in the advisory.

The maintainers reserve the right to reject reports that are not security issues, or that are not in the scope of CryFox.
For issues that are determined to not be security issues, please report them as a generic issue instead. If you choose
not to re-report the issue, the maintainers may do so themselves.

CryFox does not offer bug bounties for security issues at this time.

## Scope of Security Issues

Many security features of the web platform are not yet implemented in CryFox. Security reports regarding
incomplete features may be redirected to regular issues. The following are examples of issues that are not in scope
at this time:

- Cross-site request forgery
- Cross-site scripting
- Content Security Policy violations
- Cross-origin iframe sandboxing

The maintainers reserve the right to modify this list as the project matures and as security issues are reported.

Significant portions of the browser depend on third party libraries. Examples include image decoding, video decoding,
internationalization, and 2D graphics. Security issues in these libraries should be reported to the maintainers of the
respective libraries. The maintainers of Ladybird will work with the maintainers of these libraries to resolve the issue.
If a security issue relates more to the integration of the library into Ladybird, it should be reported via the same
methods as other security issues.

## Responsible Disclosure

The maintainers of CryFox will work with security researchers to resolve security issues in a timely manner. A default
30-day disclosure timeline is in place for all security issues, but this may be extended if the maintainers and the reporter
agree that more time is needed to resolve the issue. The maintainers will keep the reporter informed of progress and
resolution steps throughout the process.

In the case that a security issue is also reported to other browser vendors or OSS projects, the maintainers will work
with the longest disclosure timeline to ensure that all parties have sufficient time to resolve the issue.
