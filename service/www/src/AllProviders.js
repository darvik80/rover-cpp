import React from 'react';
import {BrowserRouter, Route} from 'react-router-dom';
import {QueryParamProvider} from 'use-query-params';
import ScrollToTop from './components/ScrollToTop';

const AllProviders = ({ children }) => {
  return (
    <BrowserRouter>
      <QueryParamProvider ReactRouterRoute={Route}>
        <ScrollToTop>
            {children}
        </ScrollToTop>
      </QueryParamProvider>
    </BrowserRouter>
  );
};

export default AllProviders;
