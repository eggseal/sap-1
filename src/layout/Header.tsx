import React, { Component } from "react";

import Issues from "../assets/header/Issues";
import Source from "../assets/header/Source";
import "./Header.css";

class Header extends Component {
  render = () => {
    return (
      <header>
        <div className="main-header">
          <h1 className="title">SAP-1 Simulator</h1>
          <div className="external-links">
            <a
              className="nav-btn"
              href="https://github.com/eggseal/sap-1"
              target="_blank"
              rel="noopener noreferrer"
            >
              <Source />
              <span>Source</span>
            </a>
            <a
              className="nav-btn"
              href="https://github.com/eggseal/sap-1/issues"
              target="_blank"
              rel="noopener noreferrer"
            >
              <Issues />
              <span>Issues</span>
            </a>
          </div>
        </div>
      </header>
    );
  };
}

export default Header;
