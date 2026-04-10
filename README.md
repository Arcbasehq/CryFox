# CryFox

[CryFox](https://cryfox.me) is a privacy-first, fast, and secure web browser built on an independent engine that follows web standards. The project is Linux-first, with other platforms treated as best-effort.

> [!IMPORTANT]
> CryFox is in a pre-alpha state, and only suitable for use by developers
>

## Features

- Multi-process architecture with a main UI process, per-tab renderers, and dedicated ImageDecoder/RequestServer helpers.
- Privacy-first defaults like Global Privacy Control and partitioned HTTP disk cache.
- Fast startup and navigation with HTTP memory caching and GPU painting when available.
- Security-first isolation: image decoding and network connections live out of process.

At the moment, many core library support components are inherited from SerenityOS:

- LibWeb: Web rendering engine
- LibJS: JavaScript engine
- LibWasm: WebAssembly implementation
- LibCrypto/LibTLS: Cryptography primitives and Transport Layer Security
- LibHTTP: HTTP/1.1 client
- LibGfx: 2D Graphics Library, Image Decoding and Rendering
- LibUnicode: Unicode and locale support
- LibMedia: Audio and video playback
- LibCore: Event loop, OS abstraction layer
- LibIPC: Inter-process communication

## How do I build and run this?

See [build instructions](Documentation/BuildInstructionsLadybird.md) for information on how to build CryFox.

Linux is the primary target. Other platforms may build but are not the focus.

## How do I read the documentation?

Code-related documentation can be found in the [documentation](Documentation/) folder.

## Get in touch and participate!

Join our community to participate in development discussion.

Please read [Getting started contributing](Documentation/GettingStartedContributing.md) if you plan to contribute to CryFox for the first time.

Before opening an issue, please see the [issue policy](CONTRIBUTING.md#issue-policy) and the [detailed issue-reporting guidelines](ISSUES.md).

The full contribution guidelines can be found in [`CONTRIBUTING.md`](CONTRIBUTING.md).

## License

CryFox is licensed under a 2-clause BSD license.
