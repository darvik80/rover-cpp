import React, { useState } from 'react';
import { Nav, NavItem, NavLink, TabContent, TabPane } from 'reactstrap';
import { Toolbar } from './index';
import './Tabs.scss';
import ErrorBoundary from './ErrorBoundary';

const propTypes = {};

const defaultProps = {};

const Tabs = ({ children }) => {
  const [activeTab, setActiveTab] = useState('0');
  children                        = React.Children.toArray(children).filter(a => a !== null && a !== undefined);
  const toolbar                   = children.find(a => a.type === Toolbar);
  children                        = children.filter(a => a.type !== Toolbar);

  return (
    <React.Fragment>
      <Nav tabs className="bg-white border-bottom">
        {React.Children.map(children, (el, i) => {
          return (
            <NavItem key={i}>
              <NavLink active={activeTab === String(i)} onClick={() => setActiveTab(String(i))}>
                {el.props.title}
              </NavLink>
            </NavItem>
          )
        })}
        <li className="ml-auto">
          {toolbar}
        </li>
      </Nav>
      <TabContent activeTab={activeTab}>
        {React.Children.map(children, (el, i) => {
            const active = activeTab === String(i);
            if (!active) return null; // lazy render. unmount on exit

            return (
              <TabPane tabId={String(i)} key={i}>
                <ErrorBoundary>
                  {el.props.children}
                </ErrorBoundary>
              </TabPane>
            )
          },
        )}
      </TabContent>
    </React.Fragment>
  )
};

Tabs.propTypes    = propTypes;
Tabs.defaultProps = defaultProps;

export default Tabs;
