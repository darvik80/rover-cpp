import React from 'react';
import {Nav, Navbar, NavbarBrand, NavItem} from 'reactstrap';
import './DefaultHeader.scss';

//#const propTypes = {
//};

const DefaultHeader = ({ props }) => {
  return (
    <Navbar light fixed="top" color="white" className="flex-md-nowrap p-0 shadow-sm">
      <NavbarBrand href="/" className="col-2 col-lg-1">
        <img alt="brand" src="/android-chrome-192x192.png" width="20" height="20" className="mr-2" />
        Router
      </NavbarBrand>
      <Nav className="mr-auto mr-2">
        <NavItem className="mr-1">
          test
        </NavItem>
      </Nav>
    </Navbar>
  );
};

//DefaultHeader.propTypes = propTypes;

export default DefaultHeader;
