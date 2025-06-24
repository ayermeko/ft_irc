# FT_IRC

*Empowering Seamless Communication, Unleashing Connection Potential*

![Last Commit](https://img.shields.io/badge/last%20commit-february-blue)
![Language](https://img.shields.io/badge/c%2B%2B-99.0%25-blue)
![Languages](https://img.shields.io/badge/languages-2-informational)

Built with the tools and technologies:  
![C++](https://img.shields.io/badge/C%2B%2B-blue)

---

## Table of Contents

- [Overview](#overview)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Usage](#usage)
  - [Testing](#testing)

---

## Overview

`ft_irc` is a lightweight, modular IRC server implementation crafted for developers seeking to build or extend real-time chat applications. It encapsulates core IRC functionalities, including user registration, messaging, channel management, and moderation commands, all within a maintainable architecture.

### Why ft_irc?

This project aims to provide a solid foundation for creating customizable IRC servers with reliable protocol compliance. The core features include:

- 🧠 **Command Handling**: Implements essential IRC commands like `NICK`, `PRIVMSG`, `JOIN`, and more, enabling seamless client-server interactions.
- 🌐 **Client & Server Management**: Manages user sessions, permissions, and network communication for stable multi-user environments.
- 🧱 **Modular Design**: Uses dedicated classes for clients, channels, and server logic, promoting maintainability and scalability.
- 🚀 **Real-Time Communication**: Facilitates instant messaging, channel topics, and user invitations, supporting dynamic chat experiences.
- 🔐 **Permission & Mode Control**: Enforces access restrictions, modes, and moderation commands to ensure secure interactions.

---

## Getting Started

### Prerequisites

This project requires the following dependencies:

- **Programming Language**: C++
- **Package Manager**: [CMake](https://cmake.org)

---

### Installation

Build `ft_irc` from the source and install dependencies:

1. **Clone the repository**:
   ```bash
   git clone https://github.com/ayermeko/ft_irc
   ### Installation

2. **Navigate to the project directory**:
   ```bash
   cd ft_irc
   
### Usage

Once installed, run the IRC server with the desired configuration. You can connect to it using any standard IRC client (e.g., HexChat, irssi).

**Basic command-line usage:**

```bash
./irc_server <port> <password>

