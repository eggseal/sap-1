import React, { Component } from "react";

import "./NavBar.css"

class NavBar extends Component {
  render = () => {
    return <nav className="navbar">
        <div className="nav-content">
            <a className="nav-element" href="/">Home</a>
            <a className="nav-element" href="/components">Components</a>
            <a className="nav-element" href="/simulator">Simulator</a>
        </div>
    </nav>;
  };
}

export default NavBar;
