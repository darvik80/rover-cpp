import React from 'react';
import {NavLink as RouterLink} from 'react-router-dom';
import {Nav, NavItem, NavLink,} from 'reactstrap';
import PropTypes from 'prop-types';

const propTypes = {
    navigation: PropTypes.shape({
        items: PropTypes.array,
    }),
    innerWrapperClassName: PropTypes.string,
};
const defaultProps = {};

const NavMenu = ({navigation, innerWrapperClassName, ...attributes}) => {

    return (
        <Nav {...attributes}>
            <div className={innerWrapperClassName}>
                {navigation.items.map((el, i) => {
                    if (el.divider) {
                        return (
                            <NavItem key={i}>
                                <hr/>
                            </NavItem>
                        );
                    }
                })}
            </div>
        </Nav>
    );

};

NavMenu.propTypes = propTypes;
NavMenu.defaultProps = defaultProps;

export default NavMenu;
